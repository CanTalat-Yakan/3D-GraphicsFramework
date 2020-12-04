#include <windows.h>
#include "GScene.h"
#include "CEngine.h"

int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, INT _nCmdShow)
{
#pragma region //Init
	CEngine engine = {};
	engine.Init(_hInstance, _nCmdShow);

	GScene mainScene = {};
	mainScene.Init();
#pragma endregion

	mainScene.Awake();
	mainScene.Start();

	while (engine.p_window->Run())
	{
		engine.p_d3d->Clear();

		engine.Update();

		mainScene.EarlyUpdate();
		mainScene.Update();
		mainScene.LateUpdate();

		engine.p_d3d->Present();
	}

#pragma region //Release
	mainScene.Release();
	engine.Release();
#pragma endregion

	return 0;
}