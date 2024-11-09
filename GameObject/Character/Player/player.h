#pragma once
#include "GameObject/Character/character.h"
#include <unordered_map>

class Block;
class BulletManager;
class PlayerState;
#if _DEBUG
constexpr int PLAYER_JUMP_FORCE = 5;
constexpr int PLAYER_MOVE_SPEED = 350;
constexpr int MAX_DROP_SPEED = 800;
#else
constexpr int PLAYER_JUMP_FORCE = static_cast<int>(5 * SCALING_FACTOR);
constexpr int PLAYER_MOVE_SPEED = static_cast<int>(350 * SCALING_FACTOR);
constexpr int MAX_DROP_SPEED = static_cast<int>(800 * SCALING_FACTOR);
#endif // _DEBUG


enum class PLAYER_STATE 
{
	IDLE,
	JUMP,
	DAMAGED,
	WALK,
	AIR,
	SHOOT,
	MAX_STATE
};
class Player :public Character 
{
private:
	bool		m_IsGround = false;
	bool		m_IsJumped = false;
	bool		m_IsAir = false;
	bool		m_IsHitWall = false;
	bool		m_IsDamagedJump = false;
	float		m_DeltaTime = 0;
	float		m_BlinkTime = 0;
	PlayerState* m_CurrentState{};
	BulletManager* m_Gun{};
	std::unordered_map<PLAYER_STATE, PlayerState*> m_PlayerState;
	std::vector<Block*>	m_BlockCollisionList;
	
public:
	Player(){}
	~Player(){}
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;
	virtual void Draw(const int cameraOffsetY)override;
	virtual void Uninit()override;
	void ChangeState(PLAYER_STATE state);
	int GetJumpForce() const { return PLAYER_JUMP_FORCE; }
	int GetMoveSpeed() const { return PLAYER_MOVE_SPEED; }
	int GetDropSpeed() const { return MAX_DROP_SPEED; }
	PlayerState* GetCurrentPlayerState()const { return m_CurrentState; }
	void SetIsGround(bool flag) { m_IsGround = flag; }
	bool GetIsGround() const { return m_IsGround; }
	bool GetIsJumped()const { return m_IsJumped; }
	bool GetIsDamaged()const { return m_IsDamaged; }
	void SetIsJumped(bool flag) { m_IsJumped = flag; }
	bool GetIsHitWall()const { return m_IsHitWall; }
	bool GetIsAir()const { return m_IsAir; }
	void SetIsAir(bool flag) { m_IsAir = flag; }
	BulletManager* GetGun()const { return m_Gun; }
	float GetDeltaTime()const { return m_DeltaTime; }
	virtual void TakeDamage(int dmg)override;
	void ResetPlayer();
	void CollisionDetection();
	void InvincibleColldownUpdate(const float& deltaTime);
	void MovementCalculation(const float& deltaTime);
	
};