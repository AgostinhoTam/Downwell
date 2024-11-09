#include "Manager/inputManager.h"
#include "Manager/soundManager.h"
#include "playerStateJump.h"
#if _DEBUG
constexpr int INIT_JUMP_FORCE = 500;
#else
constexpr int INIT_JUMP_FORCE = static_cast<int>(500*SCALING_FACTOR);
#endif // _DEBUG


void PlayerStateJump::Init()
{
	if (!m_Player)return;
	SoundManager::PlaySE(SOUND::SE_JUMP);
	m_Player->SetIsJumped(true);
	m_Player->SetIsGround(false);
	m_Player->SetVelocityY(-INIT_JUMP_FORCE);
}

void PlayerStateJump::Update()
{
	if (!m_Player)return;

	if (m_Player->GetIsGround())
	{
		m_Player->ChangeState(PLAYER_STATE::IDLE);
		return;
	}
	if (m_Player->GetIsAir())
	{
		m_IsSpaceButtonEnable = true;
		m_Player->ChangeState(PLAYER_STATE::AIR);
		return;
	}

	int moveSpeed = m_Player->GetMoveSpeed();

	//	左右移動対応
	if (InputManager::KeyPressed(KEY_INPUT_A))
	{
		m_Player->SetVelocityX(-moveSpeed);
	}
	else if (InputManager::KeyPressed(KEY_INPUT_D))
	{
		m_Player->SetVelocityX(moveSpeed);
	}



	//	一定フレーム数超えたらフラグ
	if (InputManager::KeyPressed(KEY_INPUT_SPACE) >= InputManager::MAX_PRESSEDFRAME && m_Player->GetVelocity().y >= 0)
	{
		m_IsSpaceButtonEnable = false;
   		m_Player->ChangeState(PLAYER_STATE::AIR);
		return;
	}
	else if (InputManager::KeyReleased(KEY_INPUT_SPACE))
	{
		m_IsSpaceButtonEnable = true;
		m_Player->ChangeState(PLAYER_STATE::AIR);
		return;
	}

	//　ジャンプする
	int jumpForce = m_Player->GetJumpForce();
	if (InputManager::KeyPressed(KEY_INPUT_SPACE) < InputManager::MAX_PRESSEDFRAME)
	{
		m_Player->SetVelocityY(- INIT_JUMP_FORCE - jumpForce*InputManager::KeyPressed(KEY_INPUT_SPACE));
	}
}

void PlayerStateJump::Draw()
{
	DrawFormatString(150, 50, GetColor(255, 255, 255), "Jump :       %d", m_IsSpaceButtonEnable);
}
