#include "Main\main.h"
#include "Manager/soundManager.h"

std::unordered_map<SOUND, int> SoundManager::m_AudioPool;
SOUND SoundManager::m_CurrentPlayingBGM;

constexpr int SOUND_VOLUME_100 = 255;
constexpr int SOUND_VOLUME_90 = 255 * 90 / 100;
constexpr int SOUND_VOLUME_80 = 255 * 80 / 100;
constexpr int SOUND_VOLUME_70 = 255 * 70 / 100;
constexpr int SOUND_VOLUME_60 = 255 * 60 / 100;
constexpr int SOUND_VOLUME_50 = 255 * 50 / 100;
constexpr int SOUND_VOLUME_40 = 255 * 40 / 100;
constexpr int SOUND_VOLUME_30 = 255 * 30 / 100;
constexpr int SOUND_VOLUME_20 = 255 * 20 / 100;
constexpr int SOUND_VOLUME_10 = 255 * 10 / 100;


void SoundManager::Init()
{
	m_CurrentPlayingBGM = SOUND::NONE;
	//	初期化でテキスチャ全部読み込む
	m_AudioPool.reserve(static_cast<int>(SOUND::MAX_SOUND)-1);
	m_AudioPool.emplace(SOUND::NONE,					LoadSoundMem("Asset\\Audio\\Fail.mp3"));
	m_AudioPool.emplace(SOUND::SE_BLOCK_DESTROYED,		LoadSoundMem("Asset\\Audio\\SE_Block_Destroyed.mp3"));
	m_AudioPool.emplace(SOUND::SE_GUN_FIRE,				LoadSoundMem("Asset\\Audio\\SE_Gun_Fire.mp3"));
	m_AudioPool.emplace(SOUND::SE_GUN_FIRE_METAL,		LoadSoundMem("Asset\\Audio\\SE_Gun_Metal.mp3"));
	m_AudioPool.emplace(SOUND::SE_GUN_HIT,				LoadSoundMem("Asset\\Audio\\SE_Bullet_Hit.mp3"));
	m_AudioPool.emplace(SOUND::SE_JUMP_HIT,				LoadSoundMem("Asset\\Audio\\SE_Jump_Hit.mp3"));
	m_AudioPool.emplace(SOUND::SE_JUMP,					LoadSoundMem("Asset\\Audio\\SE_Jump.mp3"));
	m_AudioPool.emplace(SOUND::SE_LANDING,				LoadSoundMem("Asset\\Audio\\SE_Landing.mp3"));
	m_AudioPool.emplace(SOUND::SE_PLAYER_TAKE_DAMAGE,	LoadSoundMem("Asset\\Audio\\SE_Player_Take_Damage.mp3"));
	m_AudioPool.emplace(SOUND::SE_RELOAD,				LoadSoundMem("Asset\\Audio\\SE_Reload.mp3"));
	m_AudioPool.emplace(SOUND::SE_BULLET_EMPTY,			LoadSoundMem("Asset\\Audio\\SE_BulletEmpty.mp3"));
	m_AudioPool.emplace(SOUND::SE_GOAL,					LoadSoundMem("Asset\\Audio\\SE_Goal.mp3"));
	m_AudioPool.emplace(SOUND::SE_START_BUTTON,			LoadSoundMem("Asset\\Audio\\SE_Start_Buttom.mp3"));
	m_AudioPool.emplace(SOUND::SE_GAMEOVER_BUTTON,		LoadSoundMem("Asset\\Audio\\SE_Gameover_Button.mp3"));
	m_AudioPool.emplace(SOUND::SE_CHARACTER_DOWN,		LoadSoundMem("Asset\\Audio\\SE_Character_Down.mp3"));
	m_AudioPool.emplace(SOUND::BGM_GAME,				LoadSoundMem("Asset\\Audio\\BGM_Game.mp3"));
	m_AudioPool.emplace(SOUND::BGM_GAMEOVER,			LoadSoundMem("Asset\\Audio\\BGM_Gameover.mp3"));
	m_AudioPool.emplace(SOUND::BGM_TITLE,				LoadSoundMem("Asset\\Audio\\BGM_Title.mp3"));


	ChangeVolumeSoundMem(SOUND_VOLUME_70, m_AudioPool[SOUND::SE_RELOAD]);
	ChangeVolumeSoundMem(SOUND_VOLUME_90, m_AudioPool[SOUND::SE_GUN_HIT]);
	ChangeVolumeSoundMem(SOUND_VOLUME_70, m_AudioPool[SOUND::SE_GOAL]);
	ChangeVolumeSoundMem(SOUND_VOLUME_70, m_AudioPool[SOUND::BGM_TITLE]);
	ChangeVolumeSoundMem(SOUND_VOLUME_60, m_AudioPool[SOUND::BGM_GAME]);
	ChangeVolumeSoundMem(SOUND_VOLUME_80, m_AudioPool[SOUND::BGM_GAMEOVER]);
}

void SoundManager::Uninit()
{
	//　テキスチャ解放
	InitSoundMem();

	m_AudioPool.clear();
}

int SoundManager::LoadAudio(SOUND key)
{
	//	テキスチャを探す
	auto targetTexture = m_AudioPool.find(key);
	if ( targetTexture != m_AudioPool.end())
	{
		return targetTexture->second;	//　あったら使う
	}
	else 
	{
		return m_AudioPool[SOUND::NONE];	//	失敗
	}
}

bool SoundManager::PlaySE(SOUND key)
{
	return PlaySoundMem(m_AudioPool[key], DX_PLAYTYPE_BACK) == 0;
}

bool SoundManager::PlayBGM(SOUND key)
{
	m_CurrentPlayingBGM = key;
	return PlaySoundMem(m_AudioPool[key], DX_PLAYTYPE_LOOP) == 0;
}

bool SoundManager::StopAudio(SOUND key)
{
	return StopSoundMem(m_AudioPool[key])==0;
}

bool SoundManager::ChangeBGM(SOUND key)
{
	if (m_CurrentPlayingBGM == key)return true;
	if (CheckSoundMem(m_AudioPool[m_CurrentPlayingBGM]) == 1)
	{
		StopSoundMem(m_AudioPool[m_CurrentPlayingBGM]);
	}
	m_CurrentPlayingBGM = key;
	return PlaySoundMem(m_AudioPool[m_CurrentPlayingBGM], DX_PLAYTYPE_LOOP) == 0;
}
