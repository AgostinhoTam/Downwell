#pragma once
#include "Scene/scene.h"

class TitleScene :public Scene
{
private:
	int	m_TitleNameTexture = -1;
	int m_PressToStartTexture = -1;
	float	m_DeltaTime = 0;
	float	m_BlinkTime = 0;
	float	m_VisibleTime = 0;
public:
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;
	virtual void Draw()override;
	virtual void Uninit()override;
};	