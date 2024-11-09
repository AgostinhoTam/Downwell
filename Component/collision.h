#pragma once
#include "Component/component.h"

enum class HIT_DIRECTION{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NOT_HIT,
	MAX_DIRECTION
};
class CollisionComponent: virtual public Component{
public:
	XMINT2		m_size = {1,1};
	XMINT2		m_offset = {0,0};
	HIT_DIRECTION	m_direction = HIT_DIRECTION::NOT_HIT;
	CollisionComponent(XMINT2 size, XMINT2 offset) :m_size(size), m_offset(offset) {}
	virtual void Init(){}
	virtual void Update() {}
	virtual void Draw(const int cameraOffsetY){}
	virtual bool IsOverlapping(const CollisionComponent* collision) const= 0;
	virtual bool IsCollisionHit(const CollisionComponent* collision)const = 0;
	HIT_DIRECTION CheckDirection(const CollisionComponent* collision)const;
	HIT_DIRECTION CheckUpDirection(const CollisionComponent* collision)const;
	HIT_DIRECTION CheckRightDirection(const CollisionComponent* collision)const;
};