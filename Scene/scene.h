#pragma once
#include <list>
#include <vector>
class GameObject;
class Camera;
class UI;


enum class GAMEOBJECT_LAYER 
{
	PLAYER,
	CAMERA,
	BLOCK,
	ENEMY,
	BULLET,
	GAMEOBJECT,
	UI,
	MAX_LAYER
};
class Scene 
{
protected:
	std::list<GameObject*> m_GameObject[static_cast<int>(GAMEOBJECT_LAYER::MAX_LAYER)];
	Camera* m_CameraRef{};
public:
	virtual void Init() {}
	virtual void Update(const float& deltaTime) {}
	virtual void Draw() {}
	virtual void Uninit();

	template<typename T>
	T* AddGameObject(GAMEOBJECT_LAYER layer)
	{
		T* gameObject = new T();
		if (gameObject)
		{
			m_GameObject[static_cast<int>(layer)].emplace_back(gameObject);
			return gameObject;
		}
		return nullptr;
	}

	template<typename T, typename... Args>
	T* AddGameObjectArg(GAMEOBJECT_LAYER layer, Args&&... args)
	{
		T* gameObject = new T(std::forward<Args>(args)...);
		if (gameObject) 
		{
			m_GameObject[static_cast<int>(layer)].emplace_back(gameObject);
			return gameObject;
		}
		return nullptr;
	}

	template<typename T>
	void GetGameObjectsList(std::vector<T*>& targetlist) 
	{
		for (int layer = 0; layer < static_cast<int>(GAMEOBJECT_LAYER::MAX_LAYER); ++layer)
		{
			for (GameObject* object : m_GameObject[layer])
			{
				T* ret = dynamic_cast<T*>(object);
				if (ret != nullptr) 
				{
					targetlist.emplace_back(ret);
				}
			}
		}
	}
	//　リスト指定で処理が早い
	template<typename T>
	void GetGameObjectsByLayer(std::vector<T*>& targetlist,GAMEOBJECT_LAYER layer)
	{
		for (GameObject* object : m_GameObject[static_cast<int>(layer)])
		{
			T* gameObject = dynamic_cast<T*>(object);
			if (gameObject != nullptr)
			{
				targetlist.emplace_back(gameObject);
			}
		}
	}
	template<typename T>
	T* GetGameObject() 
	{
		for (int layer = 0; layer < static_cast<int>(GAMEOBJECT_LAYER::MAX_LAYER); ++layer)
		{
			for (GameObject* object : m_GameObject[layer])
			{
				T* ret = dynamic_cast<T*>(object);
				if (ret != nullptr)return ret;
			}
		}
		return nullptr;
	}

};