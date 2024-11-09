#include "GameObject/Bullet/bullet.h"
#include "Manager/sceneManager.h"
#include "Component/boxCollisionComponent.h"
#include "Manager/bulletManager.h"
#include "Manager/soundManager.h"

constexpr int MAX_BULLET = 10;

void BulletManager::Init()
{
	Scene* scene = SceneManager::GetScene();
	if (!scene)return;

	for (int bullet = 0; bullet < MAX_BULLET; ++bullet)
	{
		m_Bullets.emplace_back(scene->AddGameObject<Bullet>(GAMEOBJECT_LAYER::BULLET));
	}
	m_CurrentBulletGauge = MAX_BULLET;
}

void BulletManager::ShootBullet(const XMINT2& pos)
{
	if (m_Bullets.empty())return;

 	for (Bullet* bullet :m_Bullets)
	{
		if (!bullet)continue;

		if (!bullet->GetEnable())
		{
			SoundManager::PlaySE(SOUND::SE_GUN_FIRE);
			SoundManager::PlaySE(SOUND::SE_GUN_FIRE_METAL);
  			bullet->SetEnable(true);
			bullet->SetPosition(pos);
			BoxCollisionComponent* bulletCollision = bullet->GetCollision();
			if (bulletCollision)bulletCollision->UpdateCollision();
			--m_CurrentBulletGauge;
			break;
		}
	}
}

void BulletManager::ReloadBullet()
{
	if (m_Bullets.empty())return;

	if (m_CurrentBulletGauge != MAX_BULLET)
	{
		SoundManager::PlaySE(SOUND::SE_RELOAD);
		for (Bullet* bullet : m_Bullets)
		{
			if (!bullet)continue;
			bullet->SetEnable(false);
		}
	}
	m_CurrentBulletGauge = MAX_BULLET;
}

