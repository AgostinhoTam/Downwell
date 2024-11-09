#pragma once
#include "DxLib.h"
#include "DirectXMath.h"
#include "cmath"

//	画面サイズ
constexpr int MAX_MAPCHIP_WIDTH = 32;
constexpr int MAX_MAPCHIP_HEIGHT = 18;

//	移動速度スケーリング
constexpr float SCALING_FACTOR = static_cast<float>(1920.0f / 1280.0f);

#if _DEBUG
constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;
constexpr int BLOCK_SIZE_WIDTH = (SCREEN_WIDTH / MAX_MAPCHIP_WIDTH);
constexpr int BLOCK_SIZE_HEIGHT = (SCREEN_HEIGHT / MAX_MAPCHIP_HEIGHT);
#else
constexpr int SCREEN_WIDTH = 1920;
constexpr int SCREEN_HEIGHT = 1080;
constexpr int BLOCK_SIZE_WIDTH = (SCREEN_WIDTH / MAX_MAPCHIP_WIDTH);
constexpr int BLOCK_SIZE_HEIGHT = (SCREEN_HEIGHT / MAX_MAPCHIP_HEIGHT);
#endif

using namespace DirectX;