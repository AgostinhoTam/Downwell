#include "Manager/inputManager.h"
#include "playerStateIdle.h"
#include "Manager/bulletManager.h"

void PlayerStateIdle::Init()
{
	if (!m_Player)return;
	m_Player->SetVelocity(XMINT2(0, 0));
	m_Player->SetAccl(XMINT2(0, 0));

	BulletManager* gun = m_Player->GetGun();
	if(gun)gun->ReloadBullet();
	
}

void PlayerStateIdle::Update()
{
	if (!m_Player)return;

	//	スペースキー有効フラグリセット
	if (InputManager::KeyPressed(KEY_INPUT_SPACE)==0)
	{
		m_IsSpaceButtonEnable = true;
	}
	//　左右移動
	if (InputManager::KeyPressed(KEY_INPUT_A) || InputManager::KeyPressed(KEY_INPUT_D) && m_Player->GetIsGround())
	{
		m_Player->ChangeState(PLAYER_STATE::WALK);
	}
	//	ジャンプ
	else if (!m_Player->GetIsAir() && InputManager::KeyTriggered(KEY_INPUT_SPACE) && m_IsSpaceButtonEnable)
	{
 		m_Player->ChangeState(PLAYER_STATE::JUMP);
		m_IsSpaceButtonEnable = false;

	}
	else if (m_Player->GetVelocity().y != 0)
	{
		m_Player->ChangeState(PLAYER_STATE::AIR);
	}

}

void PlayerStateIdle::Draw()
{
	DrawFormatString(150, 50, GetColor(255, 255, 255), "Idle :       %d", m_IsSpaceButtonEnable);
}
