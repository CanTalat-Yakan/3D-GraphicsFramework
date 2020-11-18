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

	CWindow* window = {}; //Create Window
	window = &window->getInstance(); //Singleton
	if (error = window->Init(1000, 550, _hInstance, _nCmdShow) > 0) return error;

	CTime* m_time = {}; //Create DeltaTime and Timer
	m_time = &m_time->getInstance(); //Singleton
	if (error = m_time->Init() > 0) return error;

	CInput* m_input = {}; //Create InputManger
	m_input = &m_input->getInstance(); //Singleton
	if (error = m_input->Init() > 0) return error;

	CCamera* camera = {}; //Create CameraView
	camera = &camera->getInstance();//Singleton
	if (error = camera->Init() > 0) return error;

	CDirect* d3d = {}; //Create Direct3D 11
	d3d = &d3d->getInstance(); //Singleton
	if (error = d3d->Init(true) > 0) return error;

	GScene mainScene = {};
	mainScene.Init();
#pragma endregion

	mainScene.Awake();
	mainScene.Start();

	while (window->Run()) 
	{
		d3d->Clear();

		m_time->Update();
		m_input->Update();
		camera->Update();

		mainScene.Update();
		mainScene.LateUpdate();

		d3d->Present();
	}

#pragma region //Release
	window->Release();
	camera->Release();
	m_time->Release();
	m_input->Release();
	mainScene.Release();
	d3d->Release();
#pragma endregion

	return 0;
}