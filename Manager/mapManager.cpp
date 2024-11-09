#include <fstream>
#include <sstream>

#include "mapManager.h"
#include "Manager/enemyManager.h"
#include "Manager/sceneManager.h"
#include "GameObject/Block/block.h"
#include "GameObject/Character/Enemy/jellyfish.h"
#include "GameObject/Character/Enemy/snail.h"
#include <random>

constexpr int MAX_STAGES = 10;

void MapManager::Init()
{
	LoadCSVMap(MAP_STAGE_FORMAT::START,		"Asset\\Map\\map_start.csv");

	//　ステージランダム
	std::vector<int> stagenumbers;
	for (int number = 1; number <= MAX_STAGES; ++number)
	{
		stagenumbers.emplace_back(number);
	}
	std::random_device rd;
	std::mt19937 timengine(rd());
	std::shuffle(stagenumbers.begin(), stagenumbers.end(), timengine);

	//　ステージの数だけ増加
	for (int  map = 1; map <= static_cast<int>(MAP_STAGE_FORMAT::LASTSTAGE); ++map)
	{
		//シャッフルした値を読み込む
		std::string filepath = "Asset\\Map\\map_" + std::to_string(stagenumbers.at(map))+ ".csv";
		LoadCSVMap(static_cast<MAP_STAGE_FORMAT>(map), filepath.c_str());
	}

	LoadCSVMap(MAP_STAGE_FORMAT::GOAL,		"Asset\\Map\\map_goal.csv");
	GameObjectDeploy(m_MapData);
}

void MapManager::GameObjectDeploy(const std::unordered_map<MAP_STAGE_FORMAT, std::array<std::array<int, MAX_MAPCHIP_WIDTH>, MAX_MAPCHIP_HEIGHT>>& mapData)
{
	Scene* scene = SceneManager::GetScene();
	if (!scene)return;
	if (mapData.empty())return;


	
	for (const auto& stage:mapData)
	{
		//　キーと値を一時保存
		const int& stagenumber = static_cast<int>(stage.first);
		const std::array<std::array<int, MAX_MAPCHIP_WIDTH>, MAX_MAPCHIP_HEIGHT > map = stage.second;

		for (int map_h = 0; map_h < MAX_MAPCHIP_HEIGHT; ++map_h)
		{
			for (int map_w = 0; map_w < MAX_MAPCHIP_WIDTH; ++map_w)
			{
				Block* tmp_block = nullptr;
				switch (map[map_h][map_w])
				{
				case static_cast<int>(MAP_DEPLOY_TYPE::EMPTY):
					tmp_block = nullptr;
					break;
				case static_cast<int>(MAP_DEPLOY_TYPE::WALL):
					tmp_block = scene->AddGameObjectArg<Block>(GAMEOBJECT_LAYER::BLOCK, BLOCK_TYPE::WALL);
					break;
				case static_cast<int>(MAP_DEPLOY_TYPE::UNDESTROYABLE_FIELD):
					tmp_block = scene->AddGameObjectArg<Block>(GAMEOBJECT_LAYER::BLOCK, BLOCK_TYPE::UNDESTROYABLE);
					break;
				case static_cast<int>(MAP_DEPLOY_TYPE::DESTROYABLE_FIELD):
					tmp_block = scene->AddGameObjectArg<Block>(GAMEOBJECT_LAYER::BLOCK, BLOCK_TYPE::DESTROYABLE);
					break;
				case static_cast<int>(MAP_DEPLOY_TYPE::GOAL):
					tmp_block = scene->AddGameObjectArg<Block>(GAMEOBJECT_LAYER::BLOCK, BLOCK_TYPE::GOAL);
					break;
				case static_cast<int>(MAP_DEPLOY_TYPE::SNAIL):
					EnemyManager::AddEnemy(ENEMY_TYPE::SNAIL, XMINT2(BLOCK_SIZE_WIDTH * map_w, BLOCK_SIZE_HEIGHT * map_h + stagenumber * SCREEN_HEIGHT));
					break;
				case static_cast<int>(MAP_DEPLOY_TYPE::JELLYFISH):
					EnemyManager::AddEnemy(ENEMY_TYPE::JELLYFISH, XMINT2(BLOCK_SIZE_WIDTH * map_w, BLOCK_SIZE_HEIGHT * map_h + stagenumber * SCREEN_HEIGHT));
					break;
				default:
					tmp_block = nullptr;
					break;
				}
				if(tmp_block)tmp_block->SetPosition(XMINT2(BLOCK_SIZE_WIDTH * map_w, BLOCK_SIZE_HEIGHT * map_h + stagenumber * SCREEN_HEIGHT));
			}
		}
	}
}

void MapManager::LoadCSVMap(const MAP_STAGE_FORMAT& stage, const char* filename)
{
	std::string str_buf;
	std::ifstream ifs_csv_file(filename);
	if (!ifs_csv_file.is_open())return;

	HasBOM(ifs_csv_file);

	int mapChipY = 0;

	while (std::getline(ifs_csv_file,str_buf) && mapChipY < MAX_MAPCHIP_HEIGHT)
	{
		std::stringstream ss(str_buf);
		std::string chiptype;
		int mapChipX = 0;

		while (std::getline(ss, chiptype,',') && mapChipX < MAX_MAPCHIP_WIDTH)
		{
			m_MapData[stage][mapChipY][mapChipX] = std::stoi(chiptype);
			++mapChipX;
		}
		++mapChipY;
	}
}

bool MapManager::HasBOM(std::ifstream& file)
{
	//　先頭文字探す
	char bom[3];
	file.read(bom, 3);
	if (bom[0] == '\xEF' && bom[1] == '\xBB' && bom[2] == '\xBF')
	{
		return true;
	}
	file.seekg(0);
	return false;
}

const std::unordered_map<MAP_STAGE_FORMAT, std::array<std::array<int, MAX_MAPCHIP_WIDTH>, MAX_MAPCHIP_HEIGHT>>& MapManager::ResetGameStage()
{
	//　ステージランダム
	std::vector<int> stagenumbers;
	for (int number = 1; number <= MAX_STAGES; ++number)
	{
		stagenumbers.emplace_back(number);
	}
	std::random_device rd;
	std::mt19937 timengine(rd());
	std::shuffle(stagenumbers.begin(), stagenumbers.end(), timengine);

	//　ステージの数だけ増加
	for (int map = static_cast<int>(MAP_STAGE_FORMAT::STAGE1); map <= static_cast<int>(MAP_STAGE_FORMAT::LASTSTAGE); ++map)
	{
		//シャッフルした値を読み込む
		std::string filepath = "Asset\\Map\\map_" + std::to_string(stagenumbers.at(map)) + ".csv";
		LoadCSVMap(static_cast<MAP_STAGE_FORMAT>(map), filepath.c_str());
	}
	return m_MapData;
}

