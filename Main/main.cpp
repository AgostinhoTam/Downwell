#include "Main/main.h"
#include "Manager/sceneManager.h"
#include "Manager/inputManager.h"
#include "Manager/textureManager.h"
#include "Manager/soundManager.h"
#include "Scene/gameScene.h"

constexpr float MICRO_SECOND = 1000000.0f;
constexpr float MILLI_SECOND = 1000.0f;
constexpr float WAIT_TIME_MILL = 3000;
constexpr int FPS = 60;
constexpr float MAX_DELTATIME = 0.2f;



// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{

	float	deltaTime	= 0.000001f;
	int		frameCount	= 1;
	float	averageFPS	= 0.0f;
	bool	gameStart	= true;

	ChangeWindowMode(TRUE);

	SetAlwaysRunFlag(TRUE);

	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);

	SetWaitVSyncFlag(false);

	SetDrawScreen(DX_SCREEN_BACK);

	if (DxLib_Init() == -1) // �c�w���C�u��������������
	{		
		return -1;			// �G���[���N�����璼���ɏI��
	}

	//	���\�[�X�v�[��������
	TextureManager::Init();
	SoundManager::Init();


	//	Scene������
	SceneManager::Init();


	// FPS����
	LONGLONG firstTimer = GetNowHiPerformanceCount();
	LONGLONG nowTimer = GetNowHiPerformanceCount();
	LONGLONG oldTimer = GetNowHiPerformanceCount();
	//	�Q�[�����[�v
	while (ProcessMessage() == 0 && gameStart)
	{

		//FPS����
		nowTimer = GetNowHiPerformanceCount();

		deltaTime = (nowTimer - oldTimer) / MICRO_SECOND;

		oldTimer = nowTimer;

		if (frameCount == FPS) 
		{
			LONGLONG totalFrameTime = nowTimer - firstTimer;

			LONGLONG calcAverage = totalFrameTime / FPS;

			averageFPS = MICRO_SECOND / calcAverage;

			firstTimer = GetNowHiPerformanceCount();

			frameCount = 1;
		}
		else 
		{
			++frameCount;
		}
		int wait = static_cast<int>((MICRO_SECOND) / FPS * frameCount - (nowTimer - firstTimer));

		wait = static_cast<int>(wait/MILLI_SECOND);

		if (wait > 0 && wait <= WAIT_TIME_MILL) 
		{
			WaitTimer(wait);
		}

		// deltatime����ݒu
		if (deltaTime > MAX_DELTATIME)
		{
			deltaTime = MAX_DELTATIME;
		}
		
		//	���̓`�F�b�N
		InputManager::Update();

		ClearDrawScreen();



		SceneManager::Update(deltaTime);
		SceneManager::Draw();

#if _DEBUG
		//	FPS�\��
		DrawFormatString(0, 0, GetColor(255, 255, 255), "FPS :%3.1f", averageFPS);
#endif

		ScreenFlip();

		if (InputManager::KeyTriggered(KEY_INPUT_ESCAPE))
		{
			gameStart = false;
		}
	}
	SoundManager::Uninit();
	SceneManager::Uninit();
	
	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}