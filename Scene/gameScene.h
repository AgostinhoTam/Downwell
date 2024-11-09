#pragma once
#include "Scene/scene.h"
#include "GameObject/Block/block.h"
class BulletManager;
class MapManager;
class EnemyManager;
class Player;

class GameScene :public Scene 
{
private:
	BulletManager*	m_BulletManager{};
	MapManager*		m_MapManager{};
	EnemyManager*	m_EnemyManager{};
	UI*				m_UI{};
	Player*			m_Player{};
	bool			m_GameoverFlag = false;
public:
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;
	virtual void Draw()override;
	virtual void Uninit()override;
	void ResetGameStage();
	BulletManager* GetBulletManager() const{ return m_BulletManager; }
	void SetGameoverFlag(bool flag) { m_GameoverFlag = flag; }
};