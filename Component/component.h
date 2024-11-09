#pragma once
#include "GameObject/gameObject.h"

class Component {
protected:
	GameObject* m_GameObject = {};
public:
	Component() = delete;
	Component(GameObject* object) { m_GameObject = object; }
	virtual ~Component(){}
	virtual void Init() {}
	virtual void Update(const float& deltaTime) {}
	virtual void Draw() {}
	virtual void Uninit() {}
	GameObject* GetOwner() { return m_GameObject; }
};