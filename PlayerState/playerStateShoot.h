#pragma once
#include "PlayerState/playerState.h"

class PlayerStateShoot :public PlayerState 
{
private:
	float m_CoolDownDuration = 0;
public:
	PlayerStateShoot(Player* player) :PlayerState(player) {}
	virtual void Init()override;
	virtual void Update()override;
	virtual void Uninit()override {}
	virtual void Draw()override;
};