#include "Manager/inputManager.h"

int InputManager::m_Key[256];
int InputManager::m_PreviousKey[256];

void InputManager::Update()
{
	memcpy(m_PreviousKey, m_Key, sizeof(m_Key));
	char tempKey[256];	//���݃L�[�̓��͏�Ԋi�[�ꏊ
	GetHitKeyStateAll(tempKey);
	for (int i = 0; i < 256; ++i) 
	{
		if (tempKey[i] != 0)		//	�Ή����Ă�L�[�������ꂽ��	
		{
			++m_Key[i];				//	�L�[���A���ŉ�����Ă��t���[���𐔂���
		}
		else {
			m_Key[i] = 0;			//�@������Ă��Ȃ��Ȃ�0�ɕԂ�
		}
	}
	return;
}

int InputManager::KeyPressed(int key)
{
	int framecount = m_Key[key];
	framecount >= MAX_PRESSEDFRAME ? framecount = MAX_PRESSEDFRAME : framecount;	//	�Ԃ��l�̏���l��60��
#if _DEBUG
	DrawFormatString(200, 50, GetColor(255, 255, 255), "%d", framecount);
#endif // _DEBUG

	return framecount;	//	�����������t���[����Ԃ�
}

bool InputManager::KeyTriggered(int key)
{
	return (KeyPressed(key) == 1);
}

bool InputManager::KeyReleased(int key)
{
	return(m_PreviousKey[key] != 0 && m_Key[key] == 0);
}

