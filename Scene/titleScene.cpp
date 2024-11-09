#include "Manager/textureManager.h"
#include "Manager/inputManager.h"
#include "Manager/sceneManager.h"
#include "Scene/gameScene.h"
#include "UI/ui.h"
#include "titleScene.h"
#include "Manager/soundManager.h"

constexpr float BLINK_DURATION = 0.5f;
constexpr float VISIBLE_DURATION = 0.7f;
void TitleScene::Init()
{
	m_TitleNameTexture = TextureManager::LoadTexture(TEXTURE::TITLE_NAME);
	m_PressToStartTexture = TextureManager::LoadTexture(TEXTURE::TITLE_PRESSTOSTART);
	SoundManager::PlayBGM(SOUND::BGM_TITLE);
}

void TitleScene::Update(const float& deltaTime)
{
	if (InputManager::KeyTriggered(KEY_INPUT_SPACE))
	{
		SoundManager::PlaySE(SOUND::SE_START_BUTTON);
		SceneManager::SetScene<GameScene>();
	}
	m_DeltaTime = deltaTime;
	m_VisibleTime += m_DeltaTime;
}

void TitleScene::Draw()
{
	DrawExtendGraph(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, m_TitleNameTexture, TRUE);

	if (m_VisibleTime <= VISIBLE_DURATION)
	{
		DrawExtendGraph(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, m_PressToStartTexture, TRUE);
	}
	else
	{
		m_BlinkTime += m_DeltaTime;
		if (m_BlinkTime >= BLINK_DURATION)
		{
			m_VisibleTime = 0;
			m_BlinkTime = 0;
		}
	}
}

void TitleScene::Uninit()
{
	SoundManager::StopAudio(SOUND::BGM_TITLE);
}
