#pragma once
#include <windows.h>

class CWindow
{
public:
	static CWindow& getInstance() { static CWindow instance; return instance; }
	CWindow(CWindow const&) = delete;
	void operator=(CWindow const&) = delete;

	int Init(INT _width, INT _height, HINSTANCE _hInstance, INT _nCmdShow);
	bool Run();
	void Release();

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

