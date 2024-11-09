#include "Main\main.h"
#include "Manager/textureManager.h"

std::unordered_map<TEXTURE, int> TextureManager::m_TexturePool;

void TextureManager::Init()
{
	//	初期化でテキスチャ全部読み込む
	m_TexturePool.reserve(static_cast<int>(TEXTURE::MAX_TEXTURE)-1);
	m_TexturePool.emplace(TEXTURE::FAIL,				LoadGraph("Asset\\Textures\\Fail.png"));
	m_TexturePool.emplace(TEXTURE::ENEMY_JELLYFISH,		LoadGraph("Asset\\Textures\\EnemyJellyFish.png"));
	m_TexturePool.emplace(TEXTURE::ENEMY_SNAIL,			LoadGraph("Asset\\Textures\\EnemySnail.png"));
	m_TexturePool.emplace(TEXTURE::FIELD_DESTROYABLE,	LoadGraph("Asset\\Textures\\DestroyableField.png"));
	m_TexturePool.emplace(TEXTURE::FIELD_UNDESTROYABLE, LoadGraph("Asset\\Textures\\UndestroyableField.png"));
	m_TexturePool.emplace(TEXTURE::FIELD_WALL,			LoadGraph("Asset\\Textures\\Wall.png"));
	m_TexturePool.emplace(TEXTURE::FIELD_GOAL,			LoadGraph("Asset\\Textures\\Goal.png"));
	m_TexturePool.emplace(TEXTURE::GAMEOVER_TITLE,		LoadGraph("Asset\\Textures\\Gameover.png"));
	m_TexturePool.emplace(TEXTURE::TITLE_PRESSTOSTART,	LoadGraph("Asset\\Textures\\PressToStart.png"));
	m_TexturePool.emplace(TEXTURE::TITLE_NAME,			LoadGraph("Asset\\Textures\\TitleName.png"));
	m_TexturePool.emplace(TEXTURE::PLAYER_IDLE,			LoadGraph("Asset\\Textures\\Player.png"));
	m_TexturePool.emplace(TEXTURE::PLAYER_BULLET,		LoadGraph("Asset\\Textures\\PlayerBullet.png"));
	m_TexturePool.emplace(TEXTURE::PLAYER_HP,			LoadGraph("Asset\\Textures\\PlayerHP.png"));
	m_TexturePool.emplace(TEXTURE::PLAYER_BULLETGUAGE,	LoadGraph("Asset\\Textures\\BulletGauge.png"));
}

void TextureManager::Uninit()
{
	//　テキスチャ解放
	InitGraph();

	m_TexturePool.clear();
}

int TextureManager::LoadTexture(TEXTURE key)
{
	//	テキスチャを探す
	auto targetTexture = m_TexturePool.find(key);
	if ( targetTexture != m_TexturePool.end()) 
	{
		return targetTexture->second;	//　あったら使う
	}
	else 
	{
		return m_TexturePool[TEXTURE::FAIL];	//	失敗
	}
}
