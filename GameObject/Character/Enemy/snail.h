#pragma once
#include "GameObject/Character/Enemy/enemy.h"
enum class SNAIL_DIRECTION
{
	NONE,
	LEFT,
	RIGHT,
	MAX_DIRECTION
};
class Snail :public Enemy 
{
private:
	bool m_DirectionTurnFlag = false;
	SNAIL_DIRECTION m_Direction = SNAIL_DIRECTION::NONE;
public:
	Snail(){}
	~Snail(){}
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;
	virtual void Draw(const int cameraOffsetY)override;
	virtual void Uninit()override;
	virtual void ResetEnemy(const XMINT2& pos)override;
	void CollisionDetection(int& maxPos,int& minPos, bool& isGround);
};