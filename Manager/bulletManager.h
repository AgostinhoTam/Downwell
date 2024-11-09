#pragma once
#include "Main/main.h"
#include <vector>

class Bullet;

class BulletManager
{
private:
	int	m_CurrentBulletGauge = 0;
	std::vector<Bullet*>	m_Bullets;
	int	m_Texture = -1;
	XMINT2 m_Position = { 0,0 };
	XMINT2 m_Scale = { BLOCK_SIZE_WIDTH,BLOCK_SIZE_HEIGHT };
public:
	BulletManager() {}
	void Init();
	void ShootBullet(const XMINT2& pos);
	void ReloadBullet();
	~BulletManager() {}
	const int& GetCurrentBulletGauge()const { return m_CurrentBulletGauge; }
};
