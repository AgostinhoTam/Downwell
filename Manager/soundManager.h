#pragma once
#include <unordered_map>

const enum class SOUND 
{
	NONE,
	BGM_TITLE,
	BGM_GAME,
	BGM_GAMEOVER,
	SE_BLOCK_DESTROYED,
	SE_GUN_FIRE,
	SE_GUN_FIRE_METAL,
	SE_GUN_HIT,
	SE_JUMP_HIT,
	SE_JUMP,
	SE_LANDING,
	SE_PLAYER_TAKE_DAMAGE,
	SE_RELOAD,
	SE_BULLET_EMPTY,
	SE_CHARACTER_DOWN,
	SE_GOAL,
	SE_START_BUTTON,
	SE_GAMEOVER_BUTTON,
	MAX_SOUND
};

class SoundManager 
{
private:
	static std::unordered_map<SOUND, int> m_AudioPool;
	static SOUND		m_CurrentPlayingBGM;
public:
	static void Init();
	static void Uninit();
	static int LoadAudio(SOUND key);
	static bool PlaySE(SOUND key);
	static bool PlayBGM(SOUND key);
	static bool StopAudio(SOUND key);
	static bool ChangeBGM(SOUND key);
};