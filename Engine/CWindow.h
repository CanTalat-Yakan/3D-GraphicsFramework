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
	inline HINSTANCE getHInstance() { return m_hInstance; }
	inline void showCursor(bool _show) { ShowCursor(_show); }

private:
	CWindow() {}

	HWND m_hWnd = {};
	RECT m_rect = {};
	HINSTANCE m_hInstance = {};
};

