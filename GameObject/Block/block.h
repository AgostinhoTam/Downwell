#pragma once
#include "GameObject/gameObject.h"

class BoxCollisionComponent;
class Bullet;
enum class BLOCK_TYPE 
{
	EMPTY,
	WALL,
	UNDESTROYABLE,
	DESTROYABLE,
	GOAL,
	MAX_BLOCK_TYPES,
};
class Block :public GameObject
{
protected:
	bool	m_Destroyable = false;
	BLOCK_TYPE	m_BlockType = BLOCK_TYPE::EMPTY;
	BoxCollisionComponent* m_CollisionComponent = {};
public:
	Block(BLOCK_TYPE type);
	virtual void Init()override;
	virtual void Draw(const int cameraOffsetY)override;
	virtual void Uninit()override;
	BoxCollisionComponent* GetCollision() { return m_CollisionComponent; }
	const BLOCK_TYPE& GetBlockType()const { return m_BlockType; }
	void SetBlockType(BLOCK_TYPE type);
	void ActivateBlock(BLOCK_TYPE type,XMINT2 pos);
	void BlockDestroyed();
};