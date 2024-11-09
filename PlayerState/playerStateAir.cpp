#include "Manager/inputManager.h"
#include "Manager/soundManager.h"
#include "PlayerState/playerStateAir.h"
#include "Manager/bulletManager.h"
void PlayerStateAir::Init()
{
	if(!m_Player)return;
	m_Player->SetIsAir(true);
	m_Player->SetIsGround(false);
}

void PlayerStateAir::Update()
{
	if (!m_Player)return;
	BulletManager* gun = m_Player->GetGun();
	if (!gun)return;

	if (m_Player->GetIsGround())
	{
		SoundManager::PlaySE(SOUND::SE_LANDING);
   		m_Player->ChangeState(PLAYER_STATE::IDLE);
		return;
	}
	m_Player->SetVelocityX(0);

	int moveSpeed = m_Player->GetMoveSpeed();

	//	¶‰EˆÚ“®
	if (InputManager::KeyPressed(KEY_INPUT_A))
	{
		m_Player->SetVelocityX(-moveSpeed);
	}
	else if (InputManager::KeyPressed(KEY_INPUT_D))
	{
		m_Player->SetVelocityX(moveSpeed);
	}

	if (InputManager::KeyReleased(KEY_INPUT_SPACE) && !m_IsSpaceButtonEnable)
	{
		m_IsSpaceButtonEnable = true;
	}

	if (InputManager::KeyPressed(KEY_INPUT_SPACE)==1 && GetSpaceButtonEnable()) 
	{
 		if (m_Player->GetIsAir() && m_Player->GetGun()->GetCurrentBulletGauge() > 0)
		{
   			m_Player->ChangeState(PLAYER_STATE::SHOOT);
			return;
		}
	}
}

void PlayerStateAir::Draw()
{
	DrawFormatString(150, 50, GetColor(255, 255, 255), "Air :       %d", m_IsSpaceButtonEnable);
}
