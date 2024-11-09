#include "Manager/textureManager.h"
#include "GameObject/Block/block.h"
#include "GameObject/Character/Player/player.h"
#include "GameObject/Character/Enemy/snail.h"
#include "Component/boxCollisionComponent.h"
#if _DEBUG
constexpr int SNAIL_MOVE_SPEED = 100;
#else
constexpr int SNAIL_MOVE_SPEED = static_cast<int>(100 * SCALING_FACTOR);
#endif // _DEBUG

constexpr int SNAIL_MAX_HP = 3;
constexpr int SNAIL_ATTACK_DAMAGE = 1;


void Snail::Init()
{
	Enemy::Init();
	m_HP = SNAIL_MAX_HP;
	m_AttackDmg = SNAIL_ATTACK_DAMAGE;
	m_Direction = SNAIL_DIRECTION::RIGHT;
	m_EnemyType = ENEMY_TYPE::SNAIL;
	m_Texture = TextureManager::LoadTexture(TEXTURE::ENEMY_SNAIL);
	m_EnableGravity = true;
}

void Snail::Update(const float& deltaTime)
{
	if (!m_Enable)return;
	if (!m_Player)return;

	//　キャッシュX座標
	int blockMaxXPos = 0;
	int blockMinXPos = SCREEN_WIDTH;

	bool isGround = false;

	//	現在の向きで移動する
	if (m_Direction == SNAIL_DIRECTION::RIGHT)
	{
		m_Velocity.x = SNAIL_MOVE_SPEED;
	}
	else if (m_Direction == SNAIL_DIRECTION::LEFT)
	{
		m_Velocity.x = -SNAIL_MOVE_SPEED;
	}

	Enemy::Update(deltaTime);

	CollisionDetection(blockMaxXPos, blockMinXPos, isGround);

	if (!isGround)return;

	//　ブロックから踏み外せないように調整
	if (m_Direction == SNAIL_DIRECTION::RIGHT)
	{
		
		if (m_Position.x + m_Scale.x > blockMaxXPos + BLOCK_SIZE_WIDTH)
		{
			m_Direction = SNAIL_DIRECTION::LEFT;
			m_Position.x = blockMaxXPos + BLOCK_SIZE_WIDTH - m_Scale.x;

		}
	}
	else if (m_Direction == SNAIL_DIRECTION::LEFT)
	{
		if (m_Position.x < blockMinXPos)
		{
			m_Direction = SNAIL_DIRECTION::RIGHT;
			m_Position.x = blockMinXPos;

		}
	}
	m_CollisionComponent->UpdateCollision();
}

void Snail::Draw(const int cameraOffsetY)
{
	if (!m_Enable)return;
	Enemy::Draw(cameraOffsetY);
#if _DEBUG
	DrawFormatString(500, 500, GetColor(255, 255, 255), "%d %d", m_Position.x, m_Position.y);
#endif // _DEBUG

}

void Snail::Uninit()
{
	Enemy::Uninit();
}

void Snail::ResetEnemy(const XMINT2& pos)
{
	Enemy::ResetEnemy(pos);
	m_HP = SNAIL_MAX_HP;
}

void Snail::CollisionDetection(int& maxPos, int& minPos,bool& isGround)
{
	for (Block* block : m_BlockCollisionList)
	{
		if (!block)continue;
		if (!block->GetEnable())continue;
		if (!block->GetDisplayble())continue;
		BoxCollisionComponent* blockCollision = block->GetCollision();
		if (!blockCollision)continue;

		const XMINT2& blockPos = block->GetPosition();
		const XMINT2& blockScale = block->GetScale();


		if (m_CollisionComponent->IsOverlapping(blockCollision))
		{
			//	壁判定
			if (block->GetBlockType() == BLOCK_TYPE::WALL) {
				if (m_CollisionComponent->CheckRightDirection(blockCollision) == HIT_DIRECTION::RIGHT)
				{
					m_Direction = SNAIL_DIRECTION::RIGHT;
					m_Position.x = blockPos.x + blockScale.x;
				}
				else if (m_CollisionComponent->CheckRightDirection(blockCollision) == HIT_DIRECTION::LEFT)
				{
					m_Direction = SNAIL_DIRECTION::LEFT;
					m_Position.x = blockPos.x - m_Scale.x;
				}
			}
			else if (block->GetBlockType() == BLOCK_TYPE::DESTROYABLE || block->GetBlockType() == BLOCK_TYPE::UNDESTROYABLE)
			{
				if (m_CollisionComponent->CheckDirection(blockCollision) == HIT_DIRECTION::UP)
				{
					m_Position.y = blockPos.y - m_Scale.y;
					//	重力リセット
					m_Accl.x = 0;
					m_Accl.y = 0;
					m_Velocity.y = 0;
					if (m_Direction == SNAIL_DIRECTION::RIGHT)
					{
						maxPos = max(maxPos, blockPos.x);
						isGround = true;
					}
					else if (m_Direction == SNAIL_DIRECTION::LEFT)
					{
						minPos = min(minPos, blockPos.x);
						isGround = true;
					}

				}
				else if (m_CollisionComponent->CheckDirection(blockCollision) == HIT_DIRECTION::RIGHT)
				{
					m_Direction = SNAIL_DIRECTION::RIGHT;
				}
				else if (m_CollisionComponent->CheckDirection(blockCollision) == HIT_DIRECTION::LEFT)
				{
					m_Direction = SNAIL_DIRECTION::LEFT;
				}

			}
		}


	}
}
