#include <windows.h>
#include "GScene.h"
#include "GScene2.h"
#include "CEngine.h"

int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, INT _nCmdShow)
{
#pragma region //Init
	CEngine engine = {};
	engine.Init(_hInstance, _nCmdShow);

	GScene mainScene = {};
	mainScene.Init();
	GScene2 secondScene = {};
	//secondScene.Init();
#pragma endregion

	mainScene.Awake();
	mainScene.Start();
	//secondScene.Awake();
	//secondScene.Start();

	while (engine.p_window->Run())
	{
		engine.p_d3d->Clear();

		engine.Update();

		mainScene.EarlyUpdate();
		mainScene.Update();
		mainScene.LateUpdate();
		//secondScene.EarlyUpdate();
		//secondScene.Update();
		//secondScene.LateUpdate();

		engine.p_d3d->Present();
	}

#pragma region //Release
	mainScene.Release();
	//secondScene.Release();
	engine.Release();
#pragma endregion

	return 0;
}