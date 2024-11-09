#include "Component/boxCollisionComponent.h"
#include "GameObject/Bullet/bullet.h"
#include "Manager/sceneManager.h"
#include "Manager/textureManager.h"
#include "Manager/soundManager.h"
#include "block.h"

void Block::Init()
{
	m_CollisionComponent = new BoxCollisionComponent(m_Scale, XMINT2(0, 0), this);
	if (m_CollisionComponent)m_CollisionComponent->Init();
	m_Enable = true;
}

void Block::Draw(const int cameraOffsetY)
{
	if (!m_Enable)return;

	DrawExtendGraph(m_Position.x,				m_Position.y - cameraOffsetY,
					m_Position.x + m_Scale.x,	m_Position.y + m_Scale.y - cameraOffsetY,
					m_Texture,					TRUE);

	if (m_CollisionComponent)m_CollisionComponent->Draw(cameraOffsetY);
}

void Block::Uninit()
{
	delete m_CollisionComponent;
	m_CollisionComponent = nullptr;
}

void Block::SetBlockType(BLOCK_TYPE type)
{
	switch (type)
	{
	case BLOCK_TYPE::DESTROYABLE:
		m_Destroyable = true;
		m_Texture = TextureManager::LoadTexture(TEXTURE::FIELD_DESTROYABLE);
		break;
	case BLOCK_TYPE::UNDESTROYABLE:
		m_Destroyable = false;
		m_Texture = TextureManager::LoadTexture(TEXTURE::FIELD_UNDESTROYABLE);
		break;
	case BLOCK_TYPE::WALL:
		m_Destroyable = false;
		m_Texture = TextureManager::LoadTexture(TEXTURE::FIELD_WALL);
		break;
	case BLOCK_TYPE::EMPTY:
		m_Destroyable = false;
		break;
	case BLOCK_TYPE::GOAL:
		m_Destroyable = false;
		m_Texture = TextureManager::LoadTexture(TEXTURE::FIELD_GOAL);
		break;
	}
	m_BlockType = type;
}

void Block::ActivateBlock(BLOCK_TYPE type, XMINT2 pos)
{
	m_Enable = true;
	SetBlockType(type);
	m_Position = pos;
	if (m_CollisionComponent)m_CollisionComponent->UpdateCollision();
}

void Block::BlockDestroyed()
{
	SoundManager::PlaySE(SOUND::SE_BLOCK_DESTROYED);
	m_Enable = false;
}

Block::Block(BLOCK_TYPE type)
{
	SetBlockType(type);
}
