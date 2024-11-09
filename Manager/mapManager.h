#pragma once
#include "Main/main.h"
#include <unordered_map>
#include <array>
#include <string>
enum class MAP_STAGE_FORMAT
{
	START,
	STAGE1,
	STAGE2,
	STAGE3,
	STAGE4,
	LASTSTAGE,
	GOAL,
	MAX_STAGE
};
enum class MAP_DEPLOY_TYPE
{
	EMPTY,
	WALL,
	UNDESTROYABLE_FIELD,
	DESTROYABLE_FIELD,
	SNAIL,
	JELLYFISH,
	GOAL,
	MAX_DEPLOY_TYPE
};

class MapManager
{
private:
	std::unordered_map<MAP_STAGE_FORMAT, std::array<std::array<int, MAX_MAPCHIP_WIDTH>,MAX_MAPCHIP_HEIGHT>>	m_MapData;
public:
	MapManager() {}
	~MapManager() {}
	void Init();
	void GameObjectDeploy(const std::unordered_map<MAP_STAGE_FORMAT, std::array<std::array<int, MAX_MAPCHIP_WIDTH>, MAX_MAPCHIP_HEIGHT>>& mapData);
	void LoadCSVMap(const MAP_STAGE_FORMAT& stageconst, const char* filename);
	bool HasBOM(std::ifstream& file);
	const std::unordered_map<MAP_STAGE_FORMAT, std::array<std::array<int, MAX_MAPCHIP_WIDTH>, MAX_MAPCHIP_HEIGHT>>& ResetGameStage();
};
