#include "CWindow.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

int CWindow::Init(INT _width, INT _height, HINSTANCE _hInstance, INT _nCmdShow)
{
#pragma region //Get hInstance
	m_hInstance = _hInstance;
#pragma endregion

#pragma region //Describe windows class
	WNDCLASSEX wc;

	wc.cbClsExtra = NULL;				//Extra bytes after our wc structure
	wc.cbSize = sizeof(WNDCLASSEX);		//Size of our windows class
	wc.cbWndExtra = NULL;				//Extra bytes after our windows instance
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW - 3);	//Window bg color
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);		//Default mouse Icon
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			//Title bar Icon
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);		//Icon in your taskbar
	wc.hInstance = _hInstance;			//Instance to current application
	wc.lpfnWndProc = WndProc;			//Default windows procedure function
	wc.lpszClassName = L"3D_Engine";	//Name of our windows class
	wc.lpszMenuName = NULL;				//Name of the menu attached to our window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC ;	//class styles
#pragma endregion

#pragma region //Calculate window size
	m_rect = { 0, 0, _width, _height };
	DWORD style = WS_OVERLAPPEDWINDOW;
	AdjustWindowRect(&m_rect, style, false);
#pragma endregion

#pragma region //Register windows class
	if (!RegisterClassEx(&wc)) return 1;
#pragma endregion

#pragma region //Create window instance
	m_hWnd = CreateWindowEx(
		NULL, wc.lpszClassName, wc.lpszClassName,	//Extended style //Name of our windows class //Name in the title bar of our window
		style,							//Style of our window
		CW_USEDEFAULT, CW_USEDEFAULT,	//Window default pos
		m_rect.right - m_rect.left,		//Width of our window
		m_rect.bottom - m_rect.top,		//Height of our window
		NULL, NULL, _hInstance, NULL);	//Handle to parent window //Handle to a Menu //Specifies instance of current program //used for an MDI client window

	if (!m_hWnd) return 2;
#pragma endregion

#pragma region //Show and update the window
	ShowWindow(m_hWnd, _nCmdShow);
	UpdateWindow(m_hWnd);
#pragma endregion

	return 0;
}

bool CWindow::Run()
{
#pragma region //MessagePump
	MSG msg;//Create a new message structure
	ZeroMemory(&msg, sizeof(MSG));//Clear message structure to NULL

	//If there was a windows message
	if (PeekMessage(&msg, NULL, 0, UINT_MAX, PM_REMOVE))
	{
		//Translate the message
		TranslateMessage(&msg);
		//Send the message to default windows procedure
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT)
			return false;
	}
#pragma endregion

	return true;
}

void CWindow::Release()
{
}
