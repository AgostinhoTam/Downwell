#include "GameObject/Character/Player/player.h"
#include "GameObject/Block/block.h"
#include "GameObject/Character/Enemy/jellyfish.h"
#include "Manager/textureManager.h"
#include "Manager/sceneManager.h"
#include "Component/boxCollisionComponent.h"
#include <algorithm>


#if _DEBUG
constexpr int JELLYFISH_MAX_MOVE_SPEED = 100;
constexpr int JELLYFISH_MAX_ACCL = 20;
constexpr int JELLYFISH_ACCL_FORCE = 2;
constexpr float JELLYFISH_MAX_BOUNDING_FORCE = 3;
#else
constexpr int JELLYFISH_MAX_MOVE_SPEED = static_cast<int>(100 * SCALING_FACTOR);
constexpr int JELLYFISH_MAX_ACCL = static_cast<int>(20 * SCALING_FACTOR);
constexpr int JELLYFISH_ACCL_FORCE = static_cast<int>(2 * SCALING_FACTOR);
constexpr float JELLYFISH_MAX_BOUNDING_FORCE = static_cast<int>(3 * SCALING_FACTOR);
#endif // DEBUG

constexpr int JELLYFISH_MAX_HP = 3;
constexpr int JELLYFISH_ATTACK_DAMAGE = 1;


void Jellyfish::Init()
{
	Enemy::Init();
	m_HP = JELLYFISH_MAX_HP;
	m_AttackDmg = JELLYFISH_ATTACK_DAMAGE;
	m_EnemyType = ENEMY_TYPE::JELLYFISH;
	m_Texture = TextureManager::LoadTexture(TEXTURE::ENEMY_JELLYFISH);
	m_EnableGravity = false;

	if (Scene* scene = SceneManager::GetScene())
	{
		scene->GetGameObjectsByLayer<Enemy>(m_EnemyCollisionList, GAMEOBJECT_LAYER::ENEMY);
	}
}

void Jellyfish::Update(const float& deltaTime)
{
	//　早期リターン
	if (!m_Enable)return;
	if (!m_Player)return;

	//	プレイヤーの位置をキャッシュ
	const XMINT2& playerPos = m_Player->GetPosition();
	const XMINT2& playerScale = m_Player->GetScale();

	//	ベクトル
	XMFLOAT2 vector = XMFLOAT2(static_cast<float>(playerPos.x - m_Position.x),
							   static_cast<float>(playerPos.y - m_Position.y));

	//	正規化
	XMVECTOR vecNormalize = XMLoadFloat2(&vector);
	vecNormalize = XMVector2Normalize(vecNormalize);
	XMStoreFloat2(&vector, vecNormalize);

	m_Accl.x = static_cast<int>(vector.x * JELLYFISH_MAX_ACCL);
	m_Accl.y = static_cast<int>(vector.y * JELLYFISH_MAX_ACCL);

	m_Accl.x = std::clamp(m_Accl.x, -JELLYFISH_MAX_ACCL, JELLYFISH_MAX_ACCL);
	m_Accl.y = std::clamp(m_Accl.y, -JELLYFISH_MAX_ACCL, JELLYFISH_MAX_ACCL);

	Enemy::Update(deltaTime);

	//　移動加速度上限設置
	m_Velocity.x = std::clamp(m_Velocity.x, -JELLYFISH_MAX_MOVE_SPEED, JELLYFISH_MAX_MOVE_SPEED);
	m_Velocity.y = std::clamp(m_Velocity.y, -JELLYFISH_MAX_MOVE_SPEED, JELLYFISH_MAX_MOVE_SPEED);

	CollisionDetection();

}

void Jellyfish::Draw(const int cameraOffsetY)
{
	if (!m_Enable)return;
	Enemy::Draw(cameraOffsetY);
}

void Jellyfish::Uninit()
{
	Enemy::Uninit();
}

void Jellyfish::ResetEnemy(const XMINT2& pos)
{
	Enemy::ResetEnemy(pos);
	m_HP = JELLYFISH_MAX_HP;
}

void Jellyfish::CollisionDetection()
{
	if (!m_CollisionComponent)return;

	for (Enemy* enemy : m_EnemyCollisionList)
	{
		if (!enemy)continue;
		if (!enemy->GetEnable())continue;
		if (enemy == this)continue;
		BoxCollisionComponent* enemyCollision = enemy->GetCollision();
		if (!enemyCollision)continue;

		if (m_CollisionComponent->IsOverlapping(enemy->GetCollision()))
		{
			if (enemy->GetEnemyType() == ENEMY_TYPE::JELLYFISH)
			{
				m_Velocity.x = static_cast<int>(-m_Velocity.x * JELLYFISH_MAX_BOUNDING_FORCE);
				m_Velocity.y = static_cast<int>(-m_Velocity.y * JELLYFISH_MAX_BOUNDING_FORCE);
				enemy->SetVelocityX(static_cast<int>(-m_Velocity.x * JELLYFISH_MAX_BOUNDING_FORCE));
				enemy->SetVelocityY(static_cast<int>(-m_Velocity.y * JELLYFISH_MAX_BOUNDING_FORCE));							
			}
			else if (enemy->GetEnemyType() == ENEMY_TYPE::SNAIL)
			{
				m_Velocity.x = static_cast<int>(-m_Velocity.x * JELLYFISH_MAX_BOUNDING_FORCE);
				m_Velocity.y = static_cast<int>(-m_Velocity.y * JELLYFISH_MAX_BOUNDING_FORCE);			
			}
			break;
		}
	}
	for (Block* block : m_BlockCollisionList)
	{
		if (!block)continue;
		if (!block->GetEnable())continue;
		BoxCollisionComponent* blockCollision = block->GetCollision();
		if (!blockCollision)continue;
		XMINT2 const& blockPos = block->GetPosition();
		XMINT2 const& blockScale = block->GetScale();

		if (m_CollisionComponent->IsOverlapping(blockCollision))
		{
			if (m_CollisionComponent->CheckDirection(blockCollision) == HIT_DIRECTION::UP || m_CollisionComponent->CheckDirection(blockCollision) == HIT_DIRECTION::DOWN)
			{
				m_Velocity.x = static_cast<int>(-m_Velocity.x * JELLYFISH_MAX_BOUNDING_FORCE);
				m_Velocity.y = static_cast<int>(-m_Velocity.y * JELLYFISH_MAX_BOUNDING_FORCE);
				break;
			}
			else if (m_CollisionComponent->CheckDirection(blockCollision) == HIT_DIRECTION::LEFT)
			{
				m_Position.x = blockPos.x - m_Scale.x;
				break;
			}
			else if (m_CollisionComponent->CheckDirection(blockCollision) == HIT_DIRECTION::RIGHT)
			{
				m_Position.x = blockPos.x + blockScale.x;
				break;
			}
		}
	}
	m_CollisionComponent->UpdateCollision();
}
