#pragma once
#include "Main/main.h"
#include <vector>
#include <unordered_map>
#include "GameObject/Character/Enemy/enemy.h"


class EnemyManager
{
private:
	static std::vector<Enemy*>	m_EnemyPool;	
public:
	EnemyManager() {}
	~EnemyManager() {}
	static void AddEnemy(const ENEMY_TYPE& type, const XMINT2& pos);
	static void ActivateEnemy(const ENEMY_TYPE& type, const XMINT2& pos);
	static std::vector<Enemy*>& GetEnemyList(){ return m_EnemyPool; }

};

