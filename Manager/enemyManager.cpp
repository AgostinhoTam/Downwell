#include "Manager/sceneManager.h"
#include "GameObject/Character/Enemy/snail.h"
#include "GameObject/Character/Enemy/jellyfish.h"
#include "enemyManager.h"


std::vector<Enemy*>	EnemyManager::m_EnemyPool;


void EnemyManager::AddEnemy(const ENEMY_TYPE& type,const XMINT2& pos)
{
	Scene* scene = SceneManager::GetScene();
	if (!scene)return;
	Enemy* enemy = nullptr;
	
	switch (type)
	{
	case ENEMY_TYPE::SNAIL:
		enemy = scene->AddGameObject<Snail>(GAMEOBJECT_LAYER::ENEMY);
		break;
	case ENEMY_TYPE::JELLYFISH:
		enemy = scene->AddGameObject<Jellyfish>(GAMEOBJECT_LAYER::ENEMY);
		break;
	default:
		break;
	}
	if (enemy)
	{
		enemy->SetPosition(XMINT2(pos));
		enemy->Init();
		m_EnemyPool.emplace_back(enemy);
	}
}

void EnemyManager::ActivateEnemy(const ENEMY_TYPE& type, const XMINT2& pos)
{
	//　再利用
	for (Enemy* enemy : m_EnemyPool)
	{
		if (!enemy)continue;
		if (enemy->GetEnable())continue;
		if (enemy->GetEnemyType() != type)continue;
		if (enemy->GetEnemyType() == type)
		{
			enemy->ResetEnemy(pos);
			return;
		}
	}

	//　エネミーが足りない場合
	AddEnemy(type, pos);
}

