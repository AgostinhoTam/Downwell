#include "Component/boxCollisionComponent.h"
#include "Manager/textureManager.h"
#include "Manager/sceneManager.h"
#include "Manager/soundManager.h"
#include "GameObject/Character/Enemy/enemy.h"
#include "GameObject/Block/block.h"
#include "bullet.h"

constexpr float BULLET_SIZE_H_ADJUST = 0.5f;
constexpr float BULLET_SIZE_W_ADJUST = 0.5f;
constexpr int	BULLET_ATTACK_DAMAGE = 1;
constexpr float MAX_ALIVETIME = 0.4f;		//ïbêî
#if _DEBUG
constexpr int BULLET_SPEED = 10;
#else
constexpr int BULLET_SPEED = static_cast<int>(10 * SCALING_FACTOR);
#endif // _DEBUG


void Bullet::Init()
{
	m_Scale.x = static_cast<int>(m_Scale.x * BULLET_SIZE_W_ADJUST);
	m_Scale.y = static_cast<int>(m_Scale.y * BULLET_SIZE_H_ADJUST);
	m_AttackDamage = BULLET_ATTACK_DAMAGE;

	m_CollsionComponent = new BoxCollisionComponent(XMINT2(m_Scale), XMINT2(0, 0), this);
	if(m_CollsionComponent)m_CollsionComponent->Init();

	m_Texture = TextureManager::LoadTexture(TEXTURE::PLAYER_BULLET);
	m_Enable = false;

	if (Scene* scene = SceneManager::GetScene())
	{
		scene->GetGameObjectsByLayer<Enemy>(m_EnemyCollisionList,GAMEOBJECT_LAYER::ENEMY);
		scene->GetGameObjectsByLayer<Block>(m_BlockCollisionList,GAMEOBJECT_LAYER::BLOCK);
	}
}

void Bullet::Update(const float& deltaTime)
{
 	if (!m_Enable)return;
   	if (m_EnemyCollisionList.empty())return;
	if (m_BlockCollisionList.empty())return;
	if (!m_CollsionComponent)return;

	m_FrameCnt += deltaTime;

	m_Velocity.y = BULLET_SPEED;

	m_Position.y += m_Velocity.y;

    m_CollsionComponent->Update();

	if (m_FrameCnt >= MAX_ALIVETIME)
	{
		BulletDestroyed();
		return;
	}
	
	CollisionDetection();

}

void Bullet::Draw(const int cameraOffsetY)
{
	if (!m_Enable)return;
	DrawExtendGraph(m_Position.x,				m_Position.y - cameraOffsetY,
					m_Position.x + m_Scale.x,	m_Position.y + m_Scale.y - cameraOffsetY,
					m_Texture,					TRUE);

	if (m_CollsionComponent)m_CollsionComponent->Draw(cameraOffsetY);
}

void Bullet::Uninit()
{
	delete m_CollsionComponent;
	m_CollsionComponent = nullptr;
}

void Bullet::BulletDestroyed()
{
	m_Enable = false;
	m_FrameCnt = 0;
}

void Bullet::CollisionDetection()
{
	//	 ìñÇΩÇËîªíË
	if (!m_CollsionComponent)return;

	for (Enemy* enemy : m_EnemyCollisionList)
	{
		if (!enemy)continue;
		if (!enemy->GetEnable())continue;
		BoxCollisionComponent* enemyCollision = enemy->GetCollision();
		if (!enemyCollision)continue;

		if (m_CollsionComponent->IsOverlapping(enemyCollision))
		{
			BulletDestroyed();
			SoundManager::PlaySE(SOUND::SE_GUN_HIT);
			enemy->TakeDamage(m_AttackDamage);
			break;
		}
	}
	for (Block* block : m_BlockCollisionList)
	{
		if (!block)continue;
		if (!block->GetEnable())continue;
		BoxCollisionComponent* blockCollision = block->GetCollision();
		if (!blockCollision)continue;

		if (m_CollsionComponent->IsOverlapping(blockCollision))
		{
			switch (block->GetBlockType())
			{
			case BLOCK_TYPE::UNDESTROYABLE:
				BulletDestroyed();
				break;
			case BLOCK_TYPE::DESTROYABLE:
				BulletDestroyed();
				block->BlockDestroyed();
				break;
			default:
				break;
			}
			break;
		}
	}
}
