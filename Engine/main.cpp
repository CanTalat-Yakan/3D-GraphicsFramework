#include <windows.h>
#include "CWindow.h";
#include "CDirect.h";
#include "CTime.h";
#include "CInput.h";
#include "CCamera.h";
#include "CLight.h";
#include "CMesh.h";
#include "CMaterial.h";
#include "CObjLoader.h"
#include "GScene.h"

INT WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, INT _nCmdShow)
{
#pragma region //Init CoreFeatures
	int error = 0;
	int width = 1920;
	int height = 1080;
	int windowed = false;

	CWindow* window = {}; //Create Window
	window = &window->GetInstance(); //Singleton
	if (error = window->Init(width, height, _hInstance, _nCmdShow) > 0) return error;

	CTime* p_time = {}; //Create DeltaTime and Timer
	p_time = &p_time->GetInstance(); //Singleton
	if (error = p_time->Init() > 0) return error;

	CInput* m_input = {}; //Create InputManger
	m_input = &m_input->GetInstance(); //Singleton
	if (error = m_input->Init() > 0) return error;

	CCamera* camera = {}; //Create CameraView
	camera = &camera->GetInstance();//Singleton
	if (error = camera->Init() > 0) return error;

	CDirect* d3d = {}; //Create Direct3D 11
	d3d = &d3d->GetInstance(); //Singleton
	if (error = d3d->Init(windowed) > 0) return error;

	GScene mainScene = {};
	mainScene.Init();
#pragma endregion

	mainScene.Awake();
	mainScene.Start();

	while (window->Run()) 
	{
		d3d->Clear();

		p_time->Update();
		m_input->Update();
		camera->Update();

		mainScene.EarlyUpdate();
		mainScene.Update();
		mainScene.LateUpdate();

		d3d->Present();
	}

#pragma region //Release
	window->Release();
	camera->Release();
	p_time->Release();
	m_input->Release();
	mainScene.Release();
	d3d->Release();
#pragma endregion

	return 0;
}