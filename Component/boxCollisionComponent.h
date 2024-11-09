#pragma once
#include "Component/collision.h"


class BoxCollisionComponent :public CollisionComponent {
private:
	XMINT2 max = { 0,0 };
	XMINT2 min = { 0,0 };
public:
	BoxCollisionComponent(XMINT2 size, XMINT2 offset, GameObject* gameobject) :CollisionComponent(size, offset), Component(gameobject) {}
	virtual void Init()override;
	virtual void Update()override;
	virtual void Draw(const int cameraOffsetY)override;
	virtual bool IsOverlapping(const CollisionComponent* collision)const;
	virtual bool IsCollisionHit(const CollisionComponent* collision)const;
	bool IsCollisionHit(const BoxCollisionComponent* collision)const;
	bool IsOverlapping(const BoxCollisionComponent* collision)const;
	void UpdateCollision();
};