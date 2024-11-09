#pragma once
#include <vector>
#include "GameObject/Character/character.h"
class Player;
class Block;

enum class ENEMY_TYPE
{
	NONE,
	SNAIL,
	JELLYFISH,
	MAX_ENEMY_TYPE
};

class Enemy :public Character 
{
protected:
	Player*				m_Player{};
	std::vector<Block*>	m_BlockCollisionList;
	int					m_DistanceBtwPlayer = 0;
	ENEMY_TYPE			m_EnemyType = ENEMY_TYPE::NONE;
public:
	Enemy() {}
	virtual ~Enemy(){}
	virtual void Init();
	virtual void Update(const float& deltaTime);
	virtual void Draw(const int cameraOffsetY);
	virtual void Uninit();
	virtual void ResetEnemy(const XMINT2& pos);
	void CollisionDetection();
	const ENEMY_TYPE& GetEnemyType()const { return m_EnemyType; }
};