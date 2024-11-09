#include "GameObject/Character/Player/player.h"
#include "GameObject/Block/block.h"
#include "Manager/textureManager.h"
#include "Manager/inputManager.h"
#include "Manager/sceneManager.h"
#include "Component/boxCollisionComponent.h"
#include "GameObject/Character/Enemy/enemy.h"

void Enemy::Init()
{
	Character::Init();
	if (Scene* scene = SceneManager::GetScene())
	{
		m_Player = scene->GetGameObject<Player>();
		scene->GetGameObjectsByLayer<Block>(m_BlockCollisionList,GAMEOBJECT_LAYER::BLOCK);
	}
	m_Enable = true;
}

void Enemy::Update(const float& deltaTime)
{
	if (!m_Enable)return;
	if (!m_Player)return;
	if (m_BlockCollisionList.empty())return;

	if (m_EnableGravity)
	{
		m_Accl.y = m_Gravity;
	}
	
	m_Velocity.x += m_Accl.x;
	m_Velocity.y += m_Accl.y;

	//	位置更新
	m_Position.x += static_cast<int>(m_Velocity.x * deltaTime);
	m_Position.y += static_cast<int>(m_Velocity.y * deltaTime);

	//	コリジョン更新
	Character::Update(deltaTime);

	CollisionDetection();
}

void Enemy::Draw(const int cameraOffsetY)
{
	if (!m_Enable)return;
	Character::Draw(cameraOffsetY);
}

void Enemy::Uninit()
{
	if (!m_Enable)return;
	Character::Uninit();
}

void Enemy::ResetEnemy(const XMINT2& pos)
{
	SetEnable(true);
	SetPosition(pos);
	UpdateCollision();
}

void Enemy::CollisionDetection()
{
	if (!m_CollisionComponent)return;
	if (!m_Player)return;

	//　ダメージ判定
	BoxCollisionComponent* playerCollision = m_Player->GetCollision();
	if (playerCollision)
	{
		if (m_CollisionComponent->IsOverlapping(playerCollision))
		{
			switch (m_CollisionComponent->CheckDirection(playerCollision))
			{
			case HIT_DIRECTION::DOWN:	//	踏まれあ時だけダメージ与えない（プレイヤーが上方向にいる）
				break;
			default:
				m_Player->TakeDamage(m_AttackDmg);
				break;
			}
		}
	}
}
	