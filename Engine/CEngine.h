#pragma once
#include "CWindow.h";
#include "CDirect.h";
#include "CTime.h";
#include "CInput.h";
#include "CCamera.h";
#include "CObjLoader.h"

class CEngine
{
public:
	CWindow* p_window = {};	//Create Window
	CTime* p_time = {};		//Create DeltaTime and Timer
	CInput* p_input = {};	//Create InputManger
	CCamera* p_camera = {};	//Create CameraView
	CDirect* p_d3d = {};	//Create Direct3D 11

	inline int Init(HINSTANCE _hInstance, INT _nCmdShow)
	{
		int error = 0;
		int width = 1600;
		int height = 900;
		int windowed = true;

		p_window = &p_window->GetInstance(); //Singleton
		if (error = p_window->Init(width, height, _hInstance, _nCmdShow) > 0) return error;

		p_time = &p_time->GetInstance(); //Singleton
		if (error = p_time->Init() > 0) return error;

		p_input = &p_input->GetInstance(); //Singleton
		if (error = p_input->Init() > 0) return error;

		p_camera = &p_camera->GetInstance();//Singleton
		if (error = p_camera->Init() > 0) return error;

		p_d3d = &p_d3d->GetInstance(); //Singleton
		if (error = p_d3d->Init(windowed) > 0) return error;
	}

	inline void Release()
	{
		p_window->Release();
		p_camera->Release();
		p_time->Release();
		p_input->Release();
		p_d3d->Release();
	}

	inline void Update() 
	{
		p_time->Update();
		p_input->Update();
		p_camera->Update();
	}
};