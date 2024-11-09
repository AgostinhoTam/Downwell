#pragma once
class Player;
class BulletManager;


class UI 
{
private:
	int		m_BulletTexture = -1;
	int		m_HPTexture = -1;
	int		m_GameoverTexture = -1;
	int		m_StageNumber = 1;
	float	m_StageNumberUICurrentDuration = 0;
	Player* m_Player{};
	BulletManager* m_BulletManager{};
	
public:
	UI(){}
	~UI(){}
	void Init();
	void Update(const float& deltaTime);
	void Draw();
	void NextStage();
};