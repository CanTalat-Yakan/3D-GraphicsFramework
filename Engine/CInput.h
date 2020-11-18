#pragma once
#include <dinput.h>
#include "CWindow.h";

#pragma comment(lib, "dinput8.lib")

class CInput
{
public:
	static CInput& getInstance() { static CInput instance; return instance; }
	CInput(CInput const&) = delete;
	void operator=(CInput const&) = delete;

	int init();
	void Update();
	void release();

	inline DIMOUSESTATE getMouseState() { return m_mouseState; }
	inline BYTE getKeyboardState(BYTE _keyCode) { return m_keyboardState[_keyCode]; }

private:
	CInput() {}

	CWindow* m_window;

	IDirectInputDevice8* m_diMouse = nullptr;
	IDirectInputDevice8* m_diKeyboard = nullptr;

	LPDIRECTINPUT8  m_directInput;

	DIMOUSESTATE m_mouseState;
	BYTE m_keyboardState[256];
};