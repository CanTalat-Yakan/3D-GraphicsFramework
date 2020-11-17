#pragma once
#include <windows.h>

class CWindow
{
public:
	static CWindow& getInstance() { static CWindow instance; return instance; }
	CWindow(CWindow const&) = delete;
	void operator=(CWindow const&) = delete;

	int init(HINSTANCE _hInstance, INT _width, INT _height, INT _nCmdShow);
	bool run();
	void release();

	inline HWND getHWND() { return m_hWnd; }
	inline RECT getRect() { GetWindowRect(m_hWnd, &m_rect); return m_rect; }

private:
	CWindow() {}

	HWND m_hWnd = nullptr;
	RECT m_rect = {};
};

