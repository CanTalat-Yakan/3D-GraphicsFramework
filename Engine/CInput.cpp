#include "CInput.h"

int CInput::init(HINSTANCE hInstance, HWND _hWnd)
{
	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);

	m_directInput->CreateDevice(GUID_SysKeyboard, &DIKeyboard, NULL);
	m_directInput->CreateDevice(GUID_SysMouse, &DIMouse, NULL);

	DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	DIKeyboard->SetCooperativeLevel(_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	DIMouse->SetDataFormat(&c_dfDIMouse);
	DIMouse->SetCooperativeLevel(_hWnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY);// | DISCL_FOREGROUND);

	return 0;
}

void CInput::update()
{
	DIMouse->Acquire();
	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseState);
	DIKeyboard->Acquire();
	DIKeyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);

	//if (m_keyboardState[DIK_LEFT] & 0x80)
	//if (m_mouseState.lX)

	return;
}

void CInput::release()
{
}
