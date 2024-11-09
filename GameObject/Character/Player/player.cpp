#include "Scene/gameScene.h"
#include "Manager/textureManager.h"
#include "Manager/inputManager.h"
#include "Manager/sceneManager.h"
#include "Manager/bulletManager.h"
#include "Manager/soundManager.h"
#include "GameObject/Block/block.h"
#include "GameObject/Character/Enemy/enemy.h"
#include "Component/boxCollisionComponent.h"
#include "PlayerState/playerState.h"
#include "PlayerState/playerStateIdle.h"
#include "PlayerState/playerStateWalk.h"
#include "PlayerState/playerStateJump.h"
#include "PlayerState/playerStateAir.h"
#include "PlayerState/playerStateShoot.h"
#include "GameObject/Character/Player/player.h"
#include <algorithm>

#if _DEBUG
constexpr int PLAYER_DAMAGED_JUMP_FORCE = 700;
#else
constexpr int PLAYER_DAMAGED_JUMP_FORCE = static_cast<int>(700 * SCALING_FACTOR);
#endif // _DEBUG
constexpr int PLAYER_MAX_HP = 4;
constexpr int PLAYER_ATTACKDAMAGE = 999;
constexpr float PLAYER_INVINCIBLE_TIME = 1.5f;
constexpr float BLINK_DURATION = 0.1f;


void Player::Init()
{
	Character::Init();
	m_Texture = TextureManager::LoadTexture(TEXTURE::PLAYER_IDLE);
	m_HP = PLAYER_MAX_HP;
	m_AttackDmg = PLAYER_ATTACKDAMAGE;
	m_EnableGravity = true;
	m_Enable = true;

	m_PlayerState.reserve(static_cast<int>(PLAYER_STATE::MAX_STATE)-1);
	m_PlayerState.emplace(PLAYER_STATE::IDLE,	new PlayerStateIdle(this));
	m_PlayerState.emplace(PLAYER_STATE::WALK,	new PlayerStateWalk(this));
	m_PlayerState.emplace(PLAYER_STATE::JUMP,	new PlayerStateJump(this));
	m_PlayerState.emplace(PLAYER_STATE::AIR,	new PlayerStateAir(this));
	m_PlayerState.emplace(PLAYER_STATE::SHOOT,	new PlayerStateShoot(this));

	m_CurrentState = m_PlayerState[PLAYER_STATE::AIR];
	m_CurrentState->Init();
	m_CurrentState->SetSpaceButtonEnable(true);

	GameScene* scene = dynamic_cast<GameScene*>(SceneManager::GetScene());
	if (scene)
	{
		scene->GetGameObjectsByLayer<Block>(m_BlockCollisionList,GAMEOBJECT_LAYER::BLOCK);
		scene->GetGameObjectsByLayer<Enemy>(m_EnemyCollisionList, GAMEOBJECT_LAYER::ENEMY);
		m_Gun = scene->GetBulletManager();
	}
}

void Player::Update(const float& deltaTime)
{
	if (!m_Enable)return;
	if (!m_CurrentState)return;
	if (!m_CollisionComponent)return;
	if (!m_Gun)return;

	m_DeltaTime = deltaTime;

	InvincibleColldownUpdate(deltaTime);

	m_CurrentState->Update();

	MovementCalculation(deltaTime);

	CollisionDetection();
	
	Character::Update(deltaTime);
}

void Player::Draw(const int cameraOffsetY)
{
	if (!m_CurrentState)return;
	if (m_IsDamaged)
	{
		m_BlinkTime += m_DeltaTime;
		if (static_cast<int>(m_BlinkTime / BLINK_DURATION) % 2 == 0)
		{
			Character::Draw(cameraOffsetY);
		}
		if (m_BlinkTime >= PLAYER_INVINCIBLE_TIME)
		{
			m_BlinkTime = 0;
		}
	}
	else
	{
		Character::Draw(cameraOffsetY);
	}

#if _DEBUG
	m_CurrentState->Draw();
	DrawFormatString(m_Position.x, m_Position.y-cameraOffsetY-10, GetColor(255, 255, 255), "%d %d", m_Velocity.x, m_Velocity.y);
#endif
}

void Player::Uninit()
{
	Character::Uninit();
}

void Player::ChangeState(PLAYER_STATE state)
{
	if (!m_CurrentState)return;
	//	同じだったら変換しない
	if (m_CurrentState == m_PlayerState[state])return;
	
	//	別のステートだったら
	if (m_CurrentState != m_PlayerState[state])
	{
		m_CurrentState->Uninit();
		PlayerState* nextState = m_PlayerState[state];
		if (nextState) 
		{
			m_CurrentState = nextState;
			m_CurrentState->Init();
		}
	}
}

void Player::TakeDamage(int dmg)
{
	if (m_InvincibleFrame > PLAYER_INVINCIBLE_TIME)
	{
		m_IsDamaged = true;
		m_IsDamagedJump = true;
		SoundManager::PlaySE(SOUND::SE_PLAYER_TAKE_DAMAGE);
		Character::TakeDamage(dmg);
		m_InvincibleFrame = 0;
	}
}

void Player::ResetPlayer()
{
	m_Position = XMINT2(static_cast<int>(SCREEN_WIDTH * 0.5f - m_Scale.x*0.5f), 0);
	m_EnableGravity = true;
	m_Enable = true;
	if (m_CollisionComponent)m_CollisionComponent->UpdateCollision();
	GameScene* scene = dynamic_cast<GameScene*>(SceneManager::GetScene());
	if (scene)
	{
		scene->GetGameObjectsByLayer<Block>(m_BlockCollisionList, GAMEOBJECT_LAYER::BLOCK);
		scene->GetGameObjectsByLayer<Enemy>(m_EnemyCollisionList, GAMEOBJECT_LAYER::ENEMY);
		m_Gun = scene->GetBulletManager();
		if (m_Gun)
		{
			m_Gun->ReloadBullet();
		}
	}
}

void Player::CollisionDetection()
{
	if (!m_CollisionComponent)return;
	if (!m_Gun)return;

	bool	isGoal = false;
	bool isSideHit = false;

	m_IsHitWall = false;
	m_IsGround = false;

	//	ブロックへの当たり判定
	for (Block* block : m_BlockCollisionList)
	{
		//　nullチェック
		if (!block)continue;
		if (!block->GetEnable())continue;
		BoxCollisionComponent* blockCollision = block->GetCollision();
		if (!blockCollision)continue;

		m_CollisionComponent->Update();

		bool	upCollision = false;
		const XMINT2& blockPos = block->GetPosition();
		const XMINT2& blockScale = block->GetScale();

		//　壁はOverlapping判定
		if (block->GetBlockType() == BLOCK_TYPE::WALL) {
			if (blockCollision->IsOverlapping(this->m_CollisionComponent))
			{
					if (m_CollisionComponent->CheckDirection(blockCollision) == HIT_DIRECTION::RIGHT)
					{
						m_Velocity.x = 0;
						m_Position.x = blockPos.x + blockScale.x;
						m_IsHitWall = true;
					}
					else if (m_CollisionComponent->CheckDirection(blockCollision) == HIT_DIRECTION::LEFT)
					{
						m_Velocity.x = 0;
						m_Position.x = blockPos.x - m_Scale.x;
						m_IsHitWall = true;
					}
			}
		}

		//　床、Goalはhit判定
		else if(block->GetBlockType() != BLOCK_TYPE::WALL)
		{
			if (blockCollision->IsCollisionHit(m_CollisionComponent))
			{
				if (block->GetBlockType() == BLOCK_TYPE::GOAL)
				{
					isGoal = true;
					break;
				}

				else if (block->GetBlockType() == BLOCK_TYPE::DESTROYABLE || block->GetBlockType() == BLOCK_TYPE::UNDESTROYABLE)
				{
					if (m_CollisionComponent->CheckDirection(blockCollision) == HIT_DIRECTION::LEFT)
					{
						m_Velocity.x = 0;
						m_Position.x = blockPos.x - m_Scale.x-1;
						m_IsGround = false;
						isSideHit = true;
						
					}
					else if (m_CollisionComponent->CheckDirection(blockCollision) == HIT_DIRECTION::RIGHT)
					{
						m_Velocity.x = 0;
						m_Position.x = blockPos.x + blockScale.x+1;
						m_IsGround = false;
						isSideHit = true;
						
					}
					if (m_CollisionComponent->CheckDirection(blockCollision) == HIT_DIRECTION::UP && !isSideHit)
					{

						m_Position.y = blockPos.y - m_Scale.y;
   						m_IsGround = true;
						m_IsJumped = false;
						m_IsAir = false;
						break;
					}
					else if (m_CollisionComponent->CheckDirection(blockCollision) == HIT_DIRECTION::DOWN && !isSideHit)
					{
						if ((m_CurrentState == m_PlayerState[PLAYER_STATE::JUMP]) || (m_CurrentState == m_PlayerState[PLAYER_STATE::SHOOT]) || (m_CurrentState == m_PlayerState[PLAYER_STATE::AIR]))
						{
							m_Velocity.y = m_Gravity;
							m_IsGround = false;
							m_IsAir = true;
							m_Position.y = blockPos.y + blockScale.y + 1;
							m_Accl.y = m_Gravity;
						}
						if (block->GetBlockType() == BLOCK_TYPE::DESTROYABLE && m_CurrentState == m_PlayerState[PLAYER_STATE::JUMP])
						{
							block->BlockDestroyed();
						}
						break;
					}
				}
				isSideHit = false;
			}
		}
	}
	if (isGoal)
	{
		GameScene* gameScene = dynamic_cast<GameScene*>(SceneManager::GetScene());
		if (gameScene)gameScene->ResetGameStage();
		return;
	}

	//	敵への当たり判定
	for (Enemy* enemy : m_EnemyCollisionList)
	{
		if (!enemy)continue;
		if (!enemy->GetEnable())continue;
		BoxCollisionComponent* enemyCollision = enemy->GetCollision();
		if (!enemyCollision)continue;

		//	当たり判定
		if (m_CollisionComponent->IsOverlapping(enemyCollision))
		{
			//	敵の種類で処理分け
			if (enemy->GetEnemyType() == ENEMY_TYPE::JELLYFISH)
			{
				if(m_CollisionComponent->CheckDirection(enemyCollision) == HIT_DIRECTION::UP || 
					m_CollisionComponent->CheckDirection(enemyCollision) == HIT_DIRECTION::LEFT || 
					m_CollisionComponent->CheckDirection(enemyCollision) == HIT_DIRECTION::RIGHT)
				{
					SoundManager::PlaySE(SOUND::SE_JUMP_HIT);
					enemy->TakeDamage(m_AttackDmg);			//　上から踏んだ時は敵にダメージを与える
					m_IsDamagedJump = true;
					m_Gun->ReloadBullet();
				}
				break;
			}
			continue;
		}
	}
}

void Player::InvincibleColldownUpdate(const float& deltaTime)
{
	//	無敵時間経過
	if (m_InvincibleFrame <= PLAYER_INVINCIBLE_TIME)
	{
		m_InvincibleFrame += deltaTime;
	}
	else
	{
		m_IsDamaged = false;
	}
}

void Player::MovementCalculation(const float& deltaTime)
{
	//	重力計算
	if (!m_IsGround)
	{
		m_Accl.y = m_Gravity;
	}

	m_Velocity.x += m_Accl.x;
	m_Velocity.y += m_Accl.y;

	m_Velocity.x = std::clamp(m_Velocity.x, -PLAYER_MOVE_SPEED, PLAYER_MOVE_SPEED);

	//　予約したダメージ反発を実行
	if (m_IsDamagedJump)
	{
		m_Velocity.y = -PLAYER_DAMAGED_JUMP_FORCE;
		m_IsDamagedJump = false;
	}
	if (m_IsHitWall)m_Velocity.x = 0;
	if (m_Velocity.y > MAX_DROP_SPEED)m_Velocity.y = MAX_DROP_SPEED;

	m_Position.x += static_cast<int>(m_Velocity.x * deltaTime);
	m_Position.y += static_cast<int>(m_Velocity.y * deltaTime);
}
