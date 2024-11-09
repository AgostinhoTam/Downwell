#include "Manager/textureManager.h"
#include "Manager/sceneManager.h"
#include "Manager/bulletManager.h"
#include "GameObject/Character/Player/player.h"
#include "ui.h"

constexpr float UI_STAGE_NUMBER_DISLAY_TIME = 1.0f;

void UI::Init()
{

		m_BulletTexture		= TextureManager::LoadTexture(TEXTURE::PLAYER_BULLETGUAGE);
		m_HPTexture			= TextureManager::LoadTexture(TEXTURE::PLAYER_HP);
		m_GameoverTexture	= TextureManager::LoadTexture(TEXTURE::GAMEOVER_TITLE);

		m_Player = SceneManager::GetScene()->GetGameObject<Player>();
		if (m_Player)m_BulletManager = m_Player->GetGun();
		SetFontSize(20);
		SetFontThickness(9);
}

void UI::Update(const float& deltaTime)
{
	if (m_StageNumberUICurrentDuration <= UI_STAGE_NUMBER_DISLAY_TIME)
	{
		m_StageNumberUICurrentDuration += deltaTime;
	}
}

void UI::Draw()
{
	if (!m_Player)return;
	if (!m_BulletManager)return;
	//　体力ゲージ
	for (int hp = 0; hp < m_Player->GetHp();++hp)
	{
 		DrawExtendGraph(static_cast<int>(SCREEN_WIDTH * 0.1f + BLOCK_SIZE_WIDTH* hp),					static_cast<int>(SCREEN_HEIGHT * 0.1f),
						static_cast<int>(SCREEN_WIDTH * 0.1f + BLOCK_SIZE_WIDTH+ hp*BLOCK_SIZE_WIDTH),	static_cast<int>(SCREEN_HEIGHT * 0.1f + BLOCK_SIZE_HEIGHT),
						m_HPTexture,																TRUE);
	}
	//	弾ゲージ
	for (int bulletNumber = 0; bulletNumber < m_BulletManager->GetCurrentBulletGauge(); ++bulletNumber)
	{
		DrawExtendGraph(static_cast<int>(SCREEN_WIDTH * 0.7f),						static_cast<int>(SCREEN_HEIGHT * 0.2f + BLOCK_SIZE_HEIGHT * bulletNumber),
						static_cast<int>(SCREEN_WIDTH * 0.7f + BLOCK_SIZE_WIDTH),	static_cast<int>(SCREEN_HEIGHT * 0.2f + BLOCK_SIZE_HEIGHT + BLOCK_SIZE_HEIGHT * bulletNumber),
						m_BulletTexture,											TRUE);
	}
	//　ゲームオーバー表示
	if (!m_Player->GetEnable())
	{
		DrawExtendGraph(BLOCK_SIZE_WIDTH * 10,	BLOCK_SIZE_HEIGHT * 6,
						BLOCK_SIZE_WIDTH * 22,	BLOCK_SIZE_HEIGHT * 12,
						m_GameoverTexture,		TRUE);
	}
	//　ステージ数
	if(m_StageNumberUICurrentDuration <= UI_STAGE_NUMBER_DISLAY_TIME)
	{
		DrawFormatString(static_cast<int>(SCREEN_WIDTH * 0.48f), static_cast<int>(SCREEN_HEIGHT * 0.4f),GetColor(255,255,255), "Stage %d ",m_StageNumber);
	}
	
}

void UI::NextStage()
{
	++m_StageNumber;
	m_StageNumberUICurrentDuration = 0;
}
