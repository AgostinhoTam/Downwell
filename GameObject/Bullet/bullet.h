#pragma once
#include "GameObject/gameObject.h"
class BoxCollisionComponent;
class Enemy;
class Block;

class Bullet:public GameObject
{
private:
	BoxCollisionComponent* m_CollsionComponent{};
	XMINT2	m_Velocity = { 0,0 };
	XMINT2	m_Accl = { 0,0 };
	float		m_FrameCnt = 0;
	int	m_AttackDamage = 0;

	std::vector<Enemy*>		m_EnemyCollisionList;
	std::vector<Block*>		m_BlockCollisionList;
public:
	Bullet(){}
	~Bullet(){}
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;
	virtual void Draw(const int cameraOffsetY)override;
	virtual void Uninit()override;
	void BulletDestroyed();
	void CollisionDetection();
	BoxCollisionComponent* GetCollision() const{ return m_CollsionComponent; }
};