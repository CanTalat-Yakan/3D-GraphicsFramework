#pragma once
#include <windows.h>

class CWindow
{
public:
	int init(HINSTANCE _hInstance, INT _width, INT _height, INT _nCmdShow);
	bool run();
	void release();

	inline HWND getHWND() { return m_hWnd; }
	inline RECT getRECT() { return m_rect; }

private:
	HWND m_hWnd = nullptr;
	RECT m_rect = {};
};

