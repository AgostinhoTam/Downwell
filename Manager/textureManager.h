#pragma once
#include <unordered_map>

const enum class TEXTURE 
{
	FAIL,
	TITLE_NAME,
	TITLE_PRESSTOSTART,
	FIELD_UNDESTROYABLE,
	FIELD_DESTROYABLE,
	FIELD_WALL,
	FIELD_GOAL,
	GAMEOVER_TITLE,
	PLAYER_IDLE,
	PLAYER_HP,
	PLAYER_BULLET,
	PLAYER_BULLETGUAGE,
	ENEMY_JELLYFISH,
	ENEMY_SNAIL,
	MAX_TEXTURE
};

class TextureManager 
{
private:
	static std::unordered_map<TEXTURE, int> m_TexturePool;
public:
	static void Init();
	static void Uninit();
	static int LoadTexture(TEXTURE key);
};