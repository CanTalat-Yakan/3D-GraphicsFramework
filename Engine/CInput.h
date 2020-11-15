#pragma once
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")

class CInput
{
public:
	static CInput& getInstance() { static CInput instance; return instance; }
	CInput(CInput const&) = delete;
	void operator=(CInput const&) = delete;

	int init(HINSTANCE hInstance, HWND _hWnd);
	void update();
	void release();

	inline DIMOUSESTATE getMouseState() { return m_mouseState; }
	inline BYTE getKeyboardState(BYTE _keyCode) { return m_keyboardState[_keyCode]; }

private:
	CInput() {}

	IDirectInputDevice8* DIMouse = nullptr;
	IDirectInputDevice8* DIKeyboard = nullptr;

	LPDIRECTINPUT8  m_directInput;

	DIMOUSESTATE m_mouseState;
	BYTE m_keyboardState[256];
};