#include "Manager/bulletManager.h"
#include "Manager/sceneManager.h"
#include "Manager/inputManager.h"
#include "Manager/mapManager.h"
#include "Manager/soundManager.h"
#include "Scene/titleScene.h"
#include "Manager/enemyManager.h"
#include "GameObject/Character/Player/player.h"
#include "GameObject/Character/Enemy/enemy.h"
#include "GameObject/Camera/camera.h"
#include "GameObject/Block/block.h"
#include "Component/boxCollisionComponent.h"
#include "UI/ui.h"
#include "Scene/gameScene.h"



void GameScene::Init()
{
	AddGameObject<Camera>((GAMEOBJECT_LAYER::CAMERA));
	m_Player = AddGameObject<Player>((GAMEOBJECT_LAYER::PLAYER));
	if(m_Player)
	{
		m_Player->SetPosition(XMINT2(static_cast<int>(SCREEN_WIDTH * 0.5f), 0));
	}

	m_EnemyManager = new EnemyManager;

	m_BulletManager = new BulletManager;
	if(m_BulletManager)m_BulletManager->Init();

	m_MapManager = new MapManager;
	if (m_MapManager)m_MapManager->Init();

	Camera* camera = GetGameObject<Camera>();
	if (camera)m_CameraRef = camera;

	for (int layer = 0; layer < static_cast<int>(GAMEOBJECT_LAYER::MAX_LAYER); ++layer)
	{

		for (GameObject* object : m_GameObject[layer])
		{
			if (object)object->Init();
		}
	}

	m_UI = new UI;
	if(m_UI)m_UI->Init();

	SoundManager::PlayBGM(SOUND::BGM_GAME);

}

void GameScene::Update(const float& deltaTime)
{
	XMINT2 displayableRange = { 0,0 };

	if (m_CameraRef)
	{
		displayableRange = m_CameraRef->GetCameraCurrentView();
	}
	for (int layer = 0; layer < static_cast<int>(GAMEOBJECT_LAYER::MAX_LAYER); ++layer)
	{
		for (GameObject* object : m_GameObject[layer])
		{
			if (!object)continue;

			int objPositionY = object->GetPosition().y;

			if (objPositionY > displayableRange.x && objPositionY < displayableRange.y)
			{
				object->SetDisplayable(true);
			}
			else {
				object->SetDisplayable(false);
			}
			if (object->GetEnable() && object->GetDisplayble())object->Update(deltaTime);
		}
	}
	m_UI->Update(deltaTime);
}

void GameScene::Draw()
{
	if (!m_Player)return;

	int cameraOffsetY = 0;
	XMINT2 cameraShakeOffset = { 0,0 };

	if (m_CameraRef)
	{
		cameraOffsetY = m_CameraRef->GetCameraOffsetY();
		cameraShakeOffset = m_CameraRef->CameraShake();
	}

	for (int layer = 0; layer < static_cast<int>(GAMEOBJECT_LAYER::MAX_LAYER); ++layer)
	{
		for (GameObject* object : m_GameObject[layer])
		{
			if (!object)continue;
			if (object->GetDisplayble())object->Draw(cameraOffsetY + cameraShakeOffset.y);
		}
	}

	m_UI->Draw();

	if (!m_Player->GetEnable())
	{
		SoundManager::ChangeBGM(SOUND::BGM_GAMEOVER);
		if (InputManager::KeyTriggered(KEY_INPUT_SPACE))
		{
			SoundManager::PlaySE(SOUND::SE_GAMEOVER_BUTTON);
			SceneManager::SetScene<TitleScene>();
			SoundManager::StopAudio(SOUND::BGM_GAMEOVER);
		}
	}
	
}

void GameScene::Uninit()
{
	SoundManager::StopAudio(SOUND::BGM_GAME);
	Scene::Uninit();

	delete m_BulletManager;
	m_BulletManager = nullptr;
	delete m_MapManager;
	m_MapManager = nullptr;
	delete m_EnemyManager;
	m_EnemyManager = nullptr;
	delete m_UI;
	m_UI = nullptr;

}

void GameScene::ResetGameStage()
{
	SoundManager::PlaySE(SOUND::SE_GOAL);

	if (m_UI) 
	{
		m_UI->NextStage();
	}
	if(m_CameraRef)m_CameraRef->Init();
	Player* player = GetGameObject<Player>();
	if (player)player->ResetPlayer();
	if (!m_MapManager)return;
	if (!m_EnemyManager)return;
	if (!m_BulletManager)return;
	const std::unordered_map<MAP_STAGE_FORMAT, std::array<std::array<int, MAX_MAPCHIP_WIDTH>, MAX_MAPCHIP_HEIGHT>>& mapData = m_MapManager->ResetGameStage();

	//	エネミーとブロックを初期化
	for (int layer = static_cast<int>(GAMEOBJECT_LAYER::BLOCK); layer <= static_cast<int>(GAMEOBJECT_LAYER::ENEMY); ++layer)
	{
		for (GameObject* gameobject : m_GameObject[layer])
		{
			gameobject->SetEnable(false);
			gameobject->SetDisplayable(false);
			gameobject->SetPosition(XMINT2(0, 0));
		}
	}
	

	for (const auto& stage : mapData)
	{
		//　キーと値を一時保存
		const int& stagenumber = static_cast<int>(stage.first);
		const std::array<std::array<int, MAX_MAPCHIP_WIDTH>, MAX_MAPCHIP_HEIGHT >& map = stage.second;

		for (int map_h = 0; map_h < MAX_MAPCHIP_HEIGHT; ++map_h)
		{
			for (int map_w = 0; map_w < MAX_MAPCHIP_WIDTH; ++map_w)
			{
				//	空っぽだったらcontinue
				if (map[map_h][map_w] == static_cast<int>(MAP_DEPLOY_TYPE::EMPTY))continue;

				//　置く位置を計算
				XMINT2 tmp_pos = XMINT2(BLOCK_SIZE_WIDTH * map_w, BLOCK_SIZE_HEIGHT * map_h + stagenumber * SCREEN_HEIGHT);

				//	フィールドの種類の処理
				if (map[map_h][map_w] == static_cast<int>(MAP_DEPLOY_TYPE::WALL) || 
					map[map_h][map_w] == static_cast<int>(MAP_DEPLOY_TYPE::UNDESTROYABLE_FIELD) || 
					map[map_h][map_w] == static_cast<int>(MAP_DEPLOY_TYPE::DESTROYABLE_FIELD) ||
					map[map_h][map_w] == static_cast<int>(MAP_DEPLOY_TYPE::GOAL))
				{
					const std::list<GameObject*>& blocks = m_GameObject[static_cast<int>(GAMEOBJECT_LAYER::BLOCK)];

					bool IsEnoughBlock = false;
					for (GameObject* gameobject : blocks)
					{
						Block* block = dynamic_cast<Block*>(gameobject);
						if (!block)continue;
						if (block->GetEnable())continue;


						switch (map[map_h][map_w])
						{
						case static_cast<int>(MAP_DEPLOY_TYPE::WALL):
							block->ActivateBlock(BLOCK_TYPE::WALL,tmp_pos);
							IsEnoughBlock = true;
							break;
						case static_cast<int>(MAP_DEPLOY_TYPE::UNDESTROYABLE_FIELD):
							block->ActivateBlock(BLOCK_TYPE::UNDESTROYABLE, tmp_pos);
							IsEnoughBlock = true;
							break;
						case static_cast<int>(MAP_DEPLOY_TYPE::DESTROYABLE_FIELD):
							block->ActivateBlock(BLOCK_TYPE::DESTROYABLE, tmp_pos);
							IsEnoughBlock = true;
							break;
						case static_cast<int>(MAP_DEPLOY_TYPE::GOAL):
							block->ActivateBlock(BLOCK_TYPE::GOAL, tmp_pos);
							IsEnoughBlock = true;
							break;
						}

						if(IsEnoughBlock)break;
					}
					if (!IsEnoughBlock)
					{
						Block* newblock = nullptr;
						switch (map[map_h][map_w])
						{
						case  static_cast<int>(MAP_DEPLOY_TYPE::WALL):
							newblock = AddGameObjectArg<Block>(GAMEOBJECT_LAYER::BLOCK, BLOCK_TYPE::WALL);
							break;
						case static_cast<int>(MAP_DEPLOY_TYPE::DESTROYABLE_FIELD):
							newblock = AddGameObjectArg<Block>(GAMEOBJECT_LAYER::BLOCK, BLOCK_TYPE::DESTROYABLE);
							break;
						case static_cast<int>(MAP_DEPLOY_TYPE::UNDESTROYABLE_FIELD):
							newblock = AddGameObjectArg<Block>(GAMEOBJECT_LAYER::BLOCK, BLOCK_TYPE::UNDESTROYABLE);
							break;
						case static_cast<int>(MAP_DEPLOY_TYPE::GOAL):
							newblock = AddGameObjectArg<Block>(GAMEOBJECT_LAYER::BLOCK, BLOCK_TYPE::GOAL);
							break;
						}
						newblock->SetPosition(tmp_pos);
						newblock->Init();
					}
				}
				else if (map[map_h][map_w] == static_cast<int>(MAP_DEPLOY_TYPE::SNAIL) || map[map_h][map_w] == static_cast<int>(MAP_DEPLOY_TYPE::JELLYFISH))
				{
					switch (map[map_h][map_w])
					{
					case static_cast<int>(MAP_DEPLOY_TYPE::SNAIL):
						EnemyManager::ActivateEnemy(ENEMY_TYPE::SNAIL, tmp_pos);
						break;
					case static_cast<int>(MAP_DEPLOY_TYPE::JELLYFISH):
						EnemyManager::ActivateEnemy(ENEMY_TYPE::JELLYFISH, tmp_pos);
						break;
					}
				}
			}

		}
	}

}

