#pragma once
#include "Main/main.h"

class InputManager 
{
private:
	static int m_Key[256];
	static int m_PreviousKey[256];
public:
	static const int MAX_PRESSEDFRAME = 15;
	static void Update();
	static int KeyPressed(int);
	static bool KeyTriggered(int);
	static bool KeyReleased(int);
};