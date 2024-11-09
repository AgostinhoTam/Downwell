#include "GameObject/gameObject.h"
#include "GameObject/Camera/camera.h"
#include "UI/ui.h"
#include "scene.h"

void Scene::Uninit()
{
	for (int layer = 0; layer < static_cast<int>(GAMEOBJECT_LAYER::MAX_LAYER); ++layer)
	{
		for (GameObject* object : m_GameObject[layer])
		{
			if (!object)continue;
			object->Uninit();
			delete object;
		}
		m_GameObject[layer].clear();
	}
}
