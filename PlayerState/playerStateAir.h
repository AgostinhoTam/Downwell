#pragma once
#include "PlayerState/playerState.h"

class PlayerStateAir :public PlayerState 
{
public:
	PlayerStateAir(Player* player) :PlayerState(player){}
	virtual void Init()override;
	virtual void Update()override;
	virtual void Uninit()override {}
	virtual void Draw()override;
};