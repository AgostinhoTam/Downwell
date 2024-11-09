#pragma once
#include "Main/main.h"
#include <vector>


class GameObject 
{
protected:
	XMINT2	m_Position = { 0,0 };
	XMINT2	m_Scale = { BLOCK_SIZE_WIDTH,BLOCK_SIZE_HEIGHT };
	XMINT2	m_Rotation = { 0,0 };
	bool		m_Enable = false;
	int			m_Texture = -1;
	bool		m_Displayable = false;
public:
	GameObject(){}
	virtual ~GameObject(){}
	virtual void Init(){}
	virtual void Update(const float& deltaTime){}
	virtual void Draw(const int cameraOffsetY){}
	virtual void Uninit(){}
	void	SetPosition(const XMINT2& pos) { m_Position = pos; }
	void	SetPositionX(const int& posx) { m_Position.x = posx; }
	void	SetPositionY(const int& posy) { m_Position.y = posy; }
	void	SetScale(const XMINT2& scale) { m_Scale = scale; }
	void	SetRotation(const XMINT2& rot) { m_Rotation = rot; }
	void	SetEnable(bool flag) { m_Enable = flag; }
	void	SetDisplayable(bool flag) { m_Displayable = flag; }
	const XMINT2&	GetPosition() const { return m_Position; }
	const XMINT2&	GetScale() const { return m_Scale; }
	const XMINT2&	GetRotation() const { return m_Rotation; }
	const int		GetTexture() const { return m_Texture; }
	const bool	GetEnable()const { return m_Enable; }
	const bool	GetDisplayble()const { return m_Displayable; }
};