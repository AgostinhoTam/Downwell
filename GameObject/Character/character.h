#pragma once
#include "GameObject/gameObject.h"

class BoxCollisionComponent;
class Enemy;

class Character :public GameObject 
{
protected:
	int		m_HP = 0;
	int		m_AttackDmg = 0;
	bool	m_EnableGravity = false;
	bool	m_EnableCollision = false;
	bool	m_IsDamaged = false;
	float	m_InvincibleFrame = 0;
	XMINT2	m_Velocity = { 0,0 };
	XMINT2	m_Accl = { 0,0 };
	int	m_Gravity = 30;
	BoxCollisionComponent* m_CollisionComponent{};
	std::vector<Enemy*>	m_EnemyCollisionList;
public:
	Character(){}
	virtual ~Character(){}
	virtual void Init();
	virtual void Update(const float& deltaTime);
	virtual void Draw(const int cameraOffsetY);
	virtual void Uninit();
	virtual bool isOverlapping(BoxCollisionComponent* collision);
	void UpdateCollision();
	BoxCollisionComponent* GetCollision() { return m_CollisionComponent; }
	const XMINT2& GetVelocity()const { return m_Velocity; }
	int GetAttackDamage()const { return m_AttackDmg; }
	int GetHp()const { return m_HP; }
	void SetVelocity(XMINT2 vel) { m_Velocity = vel; }
	void SetVelocityX(int vel){ m_Velocity.x = vel; }
	void SetVelocityY(int vel) { m_Velocity.y = vel; }
	void SetAccl(const XMINT2& accl) { m_Accl = accl; }
	void SetAcclX(int accl) { m_Accl.x = accl; }
	void SetAcclY(int accl) { m_Accl.y = accl; }
	virtual void TakeDamage(int dmg);
};