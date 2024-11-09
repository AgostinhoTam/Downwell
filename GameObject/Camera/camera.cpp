#include "camera.h"
#include "GameObject/Character/Player/player.h"
#include "Manager/sceneManager.h"
constexpr int CAMERA_SHAKE_STRENGTH = 5;
constexpr float CAMERA_SHAKE_DURATION = 0.5f;

void Camera::Init()
{
	Scene* scene = SceneManager::GetScene();
	if (!scene)return;
	m_Player = scene->GetGameObject<Player>();
	if (!m_Player)return;

	m_ShakeDuration = CAMERA_SHAKE_DURATION;
	m_Position = m_Player->GetPosition();
	m_Enable = true;
}

void Camera::Update(const float& deltaTime)
{
	if (!m_Player)return;
	m_Position.y = static_cast<int>(m_Player->GetPosition().y - SCREEN_HEIGHT * 0.5f);
}

XMINT2 Camera::GetCameraCurrentView() const
{
	if (!m_Player)return XMINT2(0,0);
	//	xÇÕè„ÅAyÇÕâ∫
	XMINT2 screenSize = { static_cast<int>(m_Position.y - SCREEN_HEIGHT*0.1f),static_cast<int>(m_Position.y + SCREEN_HEIGHT*1.1f) };
	return screenSize;
}

XMINT2 Camera::CameraShake()
{
	if (!m_Player) return XMINT2(0,0);

	XMINT2 shakePos = { 0,0 };
	const float deltaTime = m_Player->GetDeltaTime();
	if (m_Player->GetIsDamaged())
	{
		if (m_ShakeDuration > 0)
		{
			shakePos.x += rand() % (CAMERA_SHAKE_STRENGTH * 2) - CAMERA_SHAKE_STRENGTH;
			shakePos.y += rand() % (CAMERA_SHAKE_STRENGTH * 2) - CAMERA_SHAKE_STRENGTH;
			m_ShakeDuration -= deltaTime;
		}
		else 
		{
			shakePos = { 0,0 };
		}
	}
	else
	{
		m_ShakeDuration = CAMERA_SHAKE_DURATION;
	}
	return shakePos;
}

