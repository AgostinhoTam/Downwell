#include "character.h"
#include "Component/boxCollisionComponent.h"
#include "Manager/soundManager.h"
constexpr float CHARACTER_SIZE_H_ADJUST =0.8f;
constexpr float CHARACTER_SIZE_W_ADJUST =0.8f;
#if _DEBUG
constexpr int GRAVITY = 30;
#else
constexpr int GRAVITY = static_cast<int>(30 * SCALING_FACTOR);
#endif // _DEBUG


void Character::Init()
{
	//キャラクターサイズ調整用
	m_Scale.x = static_cast<int>(m_Scale.x * CHARACTER_SIZE_W_ADJUST);
	m_Scale.y = static_cast<int>(m_Scale.y * CHARACTER_SIZE_H_ADJUST);
	m_Gravity = GRAVITY;
	m_CollisionComponent = new BoxCollisionComponent(m_Scale, XMINT2(0, 0), this);
	if (m_CollisionComponent)m_CollisionComponent->Init();
	m_EnableCollision = true;
}

void Character::Update(const float& deltaTime)
{
	if (m_CollisionComponent)m_CollisionComponent->Update();
}

void Character::Draw(const int cameraOffsetY)
{

	DrawExtendGraph(m_Position.x,				m_Position.y - cameraOffsetY,
					m_Position.x + m_Scale.x,	m_Position.y + m_Scale.y - cameraOffsetY,
					m_Texture,					TRUE);
	if (m_CollisionComponent)m_CollisionComponent->Draw(cameraOffsetY);

}

void Character::Uninit()
{
	delete m_CollisionComponent;
	m_CollisionComponent = nullptr;
}

bool Character::isOverlapping(BoxCollisionComponent* collision)
{
	if (!collision)return false;
	return m_CollisionComponent->IsOverlapping(collision);
}

void Character::UpdateCollision()
{
	if(m_CollisionComponent)m_CollisionComponent->UpdateCollision();
}

void Character::TakeDamage(int dmg)
{
	m_HP -= dmg;
	if (m_HP <= 0)
	{
		SoundManager::PlaySE(SOUND::SE_CHARACTER_DOWN);
		m_Enable = false;
	}
}
