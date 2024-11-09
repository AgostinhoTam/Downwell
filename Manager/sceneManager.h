#pragma once
#include "Scene/scene.h"
#include "UI/fade.h"
#include <unordered_map>

enum class SCENE_TYPE
{
	TITLE_SCENE,
	GAME_SCENE,
	GAMEOVER_SCENE,
	RESULT_SCENE
};

class SceneManager 
{
private:
	static Scene*	m_CurrentScene;
	static Scene*	m_NextScene;
	static Fade*	m_Fade;
public:
	static void Init();
	static void Update(const float& deltaTime);
	static void Draw();
	static void Uninit();

	static Scene* GetScene() { return m_CurrentScene; }

	template<typename T>
	static void SetScene() 
	{
		m_NextScene = new T;
	}

};