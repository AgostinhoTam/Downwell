#include "Manager/inputManager.h"
#include "Manager/bulletManager.h"
#include "PlayerState/playerStateShoot.h"

constexpr float SHOOTING_COOLDOWN = 0.1f;

void PlayerStateShoot::Init()
{
	if (!m_Player)return;
	m_CoolDownDuration += m_Player->GetDeltaTime();
}

void PlayerStateShoot::Update()
{
	if (!m_Player)return;
	BulletManager* gun = m_Player->GetGun();
	if (!gun)return;

	if (m_Player->GetIsGround())
	{
		m_Player->ChangeState(PLAYER_STATE::IDLE);
		return;
	}
	int moveSpeed = m_Player->GetMoveSpeed();

	m_CoolDownDuration += m_Player->GetDeltaTime();
	
	if (InputManager::KeyPressed(KEY_INPUT_SPACE)&& gun->GetCurrentBulletGauge() > 0)
	{
		if (m_CoolDownDuration >= SHOOTING_COOLDOWN)
		{
			m_Player->SetVelocity(XMINT2(0, 0));
			m_Player->SetAccl(XMINT2(0, 0));
			const XMINT2& playerPos = m_Player->GetPosition();
			gun->ShootBullet(playerPos);
			m_CoolDownDuration = 0;
		}
	}
	else if (InputManager::KeyReleased(KEY_INPUT_SPACE) && !m_Player->GetIsGround())
	{
		m_Player->ChangeState(PLAYER_STATE::AIR);
		return;
	}

	//@¶‰EˆÚ“®
	if (InputManager::KeyPressed(KEY_INPUT_A))
	{
		m_Player->SetVelocityX(-moveSpeed);
	}
	else if (InputManager::KeyPressed(KEY_INPUT_D))
	{
		m_Player->SetVelocityX(moveSpeed);
	}

}

void PlayerStateShoot::Draw()
{
	DrawFormatString(150, 50, GetColor(255, 255, 255), "Shoot :       %d", m_IsSpaceButtonEnable);
}
