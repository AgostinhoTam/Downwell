#pragma once
#include "GameObject/Character/Player/player.h"
#include <unordered_map>
class Player;
class PlayerState 
{
protected:
	Player* m_Player;
	bool	m_IsSpaceButtonEnable = false;
public:
	PlayerState(Player* player) :m_Player(player) {}
	virtual ~PlayerState() = default;
	virtual void Init() {}
	virtual void Update() {}
	virtual void Uninit(){}
	virtual void Draw(){}
	Player* GetPlayer()const { return m_Player; }
	bool	GetSpaceButtonEnable()const { return m_IsSpaceButtonEnable; }
	void	SetSpaceButtonEnable(bool flag) { m_IsSpaceButtonEnable = flag; }
};