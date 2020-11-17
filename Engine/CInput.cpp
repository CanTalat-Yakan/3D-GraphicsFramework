#include "CInput.h"

int CInput::init(HINSTANCE hInstance, HWND _hWnd)
{
	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);

	m_directInput->CreateDevice(GUID_SysKeyboard, &m_diKeyboard, NULL);
	m_directInput->CreateDevice(GUID_SysMouse, &m_diMouse, NULL);

	m_diKeyboard->SetDataFormat(&c_dfDIKeyboard);
	m_diKeyboard->SetCooperativeLevel(_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	m_diMouse->SetDataFormat(&c_dfDIMouse);
	m_diMouse->SetCooperativeLevel(_hWnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY);// | DISCL_FOREGROUND);

	return 0;
}

void CInput::update()
{
	m_diMouse->Acquire();
	m_diMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseState);
	m_diKeyboard->Acquire();
	m_diKeyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);

	return;
}

void CInput::release()
{
}
