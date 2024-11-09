#include "Manager/inputManager.h"

int InputManager::m_Key[256];
int InputManager::m_PreviousKey[256];

void InputManager::Update()
{
	memcpy(m_PreviousKey, m_Key, sizeof(m_Key));
	char tempKey[256];	//現在キーの入力状態格納場所
	GetHitKeyStateAll(tempKey);
	for (int i = 0; i < 256; ++i) 
	{
		if (tempKey[i] != 0)		//	対応してるキーが押されたら	
		{
			++m_Key[i];				//	キーが連続で押されてたフレームを数える
		}
		else {
			m_Key[i] = 0;			//　押されていないなら0に返す
		}
	}
	return;
}

int InputManager::KeyPressed(int key)
{
	int framecount = m_Key[key];
	framecount >= MAX_PRESSEDFRAME ? framecount = MAX_PRESSEDFRAME : framecount;	//	返す値の上限値を60に
#if _DEBUG
	DrawFormatString(200, 50, GetColor(255, 255, 255), "%d", framecount);
#endif // _DEBUG

	return framecount;	//	長押ししたフレームを返す
}

bool InputManager::KeyTriggered(int key)
{
	return (KeyPressed(key) == 1);
}

bool InputManager::KeyReleased(int key)
{
	return(m_PreviousKey[key] != 0 && m_Key[key] == 0);
}

