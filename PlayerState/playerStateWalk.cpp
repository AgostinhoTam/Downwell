#include "Manager/inputManager.h"
#include "playerStateWalk.h"
#include "Manager/bulletManager.h"

void PlayerStateWalk::Init()
{
	if (!m_Player)return;
	BulletManager* gun = m_Player->GetGun();
	if (gun)gun->ReloadBullet();
}

void PlayerStateWalk::Update()
{
	if (!m_Player)return;

	int moveSpeed = m_Player->GetMoveSpeed();
	m_Player->SetVelocity(XMINT2(0,0));

	//	�n�ʂɂ��鎞�A����������Ă��Ȃ�����IDLE
	if (m_Player->GetIsGround() && CheckHitKeyAll() == 0)
	{
		m_Player->ChangeState(PLAYER_STATE::IDLE);
		return;
	}
	//�@�n�ʂ���Ȃ��Ƃ���AIR
	if (!m_Player->GetIsGround())
	{
		m_Player->ChangeState(PLAYER_STATE::AIR);
		return;
	}

	//�@�W�����v�L�[��������
	if (InputManager::KeyPressed(KEY_INPUT_SPACE) == 1 && !m_Player->GetIsJumped())
	{
		m_Player->ChangeState(PLAYER_STATE::JUMP);
		return;
	}
	//�@���E�ړ�
	else if (InputManager::KeyPressed(KEY_INPUT_A))
	{
		m_Player->SetVelocityX(-moveSpeed);
	}
	else if (InputManager::KeyPressed(KEY_INPUT_D))
	{
		m_Player->SetVelocityX(moveSpeed);
	}
}

void PlayerStateWalk::Draw()
{
	DrawFormatString(150, 50, GetColor(255, 255, 255), "Walk :       %d", m_IsSpaceButtonEnable);

}
