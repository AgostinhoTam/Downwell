#pragma once
#include "GameObject/gameObject.h"
#include <unordered_map>
class Player;

class Camera:public GameObject
{
private:
	Player* m_Player{};
	float	m_ShakeDuration = 0;
public:
	Camera(){}
	~Camera(){}
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;
	int GetCameraOffsetY()const { return m_Position.y; }
	XMINT2 GetCameraCurrentView()const;
	XMINT2 CameraShake();
};
