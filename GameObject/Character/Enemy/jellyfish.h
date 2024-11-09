#pragma once
#include "GameObject/Character/Enemy/enemy.h"

class Jellyfish :public Enemy 
{
private:

public:
	Jellyfish(){}
	~Jellyfish(){}
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;
	virtual void Draw(const int cameraOffsetY)override;
	virtual void Uninit()override;
	virtual void ResetEnemy(const XMINT2& pos)override;
	void CollisionDetection();
};