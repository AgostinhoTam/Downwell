#include "UI/fade.h"
#include "Scene/gameScene.h"
#include "Scene/titleScene.h"
#include "Manager/sceneManager.h"

Scene*	SceneManager::m_CurrentScene;
Scene*	SceneManager::m_NextScene;
Fade*	SceneManager::m_Fade;


void SceneManager::Init() 
{
	m_CurrentScene = new TitleScene;
	if(m_CurrentScene)m_CurrentScene->Init();
	m_Fade = new Fade;
	if (!m_Fade)DrawString(0, 0, "Fade Not Initialize", GetColor(255, 255, 255));
}

void SceneManager::Update(const float& deltaTime) 
{
	if (!m_CurrentScene)return;
	if (!m_Fade)return;

	m_CurrentScene->Update(deltaTime);
	m_Fade->UpdatingFadeMode(deltaTime);
}

void SceneManager::Draw() 
{
	if (!m_CurrentScene)return;
	if (!m_Fade)return;

	m_CurrentScene->Draw();

	if (m_NextScene && m_Fade->GetFadeMode() == FADE_MODE::FADE_NONE)
	{
		m_Fade->StartFadeOut();
	}
	if(m_Fade->GetFadeMode() == FADE_MODE::FADE_FINISHED)
	{
		m_CurrentScene->Uninit();
		delete m_CurrentScene;
		m_CurrentScene = m_NextScene;
		m_CurrentScene->Init();
		m_NextScene = nullptr;
	}
	if (m_Fade->GetFadeMode() == FADE_MODE::FADE_FINISHED)
	{
		m_Fade->StartFadeIn();
	}

	if (m_Fade->GetFadeMode() != FADE_MODE::FADE_NONE)
	{
		m_Fade->Draw();
	}
}

void SceneManager::Uninit() 
{
		delete m_CurrentScene;
		m_CurrentScene = nullptr;
		delete m_Fade;
		m_Fade = nullptr;
}

