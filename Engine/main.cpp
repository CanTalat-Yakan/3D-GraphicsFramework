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

INT WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, INT _nCmdShow)
{
#pragma region //Set Variables
	int error = 0;
	int width = 1000;
	int height = 550;
	bool windowed = true;
#pragma endregion

#pragma region //Init CoreFeatures
	CWindow* window = {}; //Create Window
	window = &window->getInstance(); //Singleton
	if (error = window->init(_hInstance, width, height, _nCmdShow) > 0) return error;

	CTime* m_time = {}; //Create DeltaTime and Timer
	m_time = &m_time->getInstance(); //Singleton
	if (error = m_time->init() > 0) return error;

	CInput* m_input = {}; //Create InputManger
	m_input = &m_input->getInstance(); //Singleton
	if (error = m_input->init() > 0) return error;

	CCamera* camera = {}; //Create CameraView
	camera = &camera->getInstance();//Singleton
	if (error = camera->init() > 0) return error;

	CDirect* d3d = {}; //Create Direct3D 11
	d3d = &d3d->getInstance(); //Singleton
	if (error = d3d->init(windowed) > 0) return error;
#pragma endregion

#pragma region //Init Content
	CObjLoader obj = {};

	CMesh mesh = {}; //Create Meshes
	if (error = mesh.init(obj.load(L"R_Cube.obj")) > 0) return error;
	CMaterial material = {}; //Create Material for Mesh
	if (error = material.init(L"T_Main.jpeg", L"S_Standard.hlsl") > 0) return error;

	CMesh mesh2 = {}; //Create Meshes
	if (error = mesh2.init(obj.load(L"R_Cube.obj")) > 0) return error;
	CMaterial material2; //Create Material for Mesh
	if (error = material2.init(L"T_Proto.png", L"S_Cell.hlsl") > 0) return error;


	CLight light = {};
	light.direction = { -0.7f, -0.5f, 0.5f };
	light.ambient = { 0.1f, 0.1f, 0.1f, 1.0f };
	light.diffuse = { 1.0f, 0.9f, 0.9f, 1.0f };
	light.intensity = 0.7f;
#pragma endregion

#pragma region //Start
	mesh.setPosition(-0.8f, 0.0f, 0.0f);
	material.setLightBuffer(light);

	mesh2.setPosition(0.8f, 0.0f, 0.0f);
	material2.setLightBuffer(light);
#pragma endregion
#pragma region //Update
	while (window->run()) 
	{
		d3d->clear();

		m_time->Update();
		m_input->Update();
		camera->Update();


		mesh.Update();
		material.render(mesh.getWorldMatrix(), camera->getViewProjectionMatrix());
		mesh.render();
		d3d->getDeviceContext()->DrawIndexed(mesh.getIndexCount(), 0, 0);

		mesh2.Update();
		material2.render(mesh2.getWorldMatrix(), camera->getViewProjectionMatrix());
		mesh2.render();
		d3d->getDeviceContext()->DrawIndexed(mesh2.getIndexCount(), 0, 0);


		d3d->present();
	}
#pragma endregion

#pragma region //release
	window->release();
	camera->release();
	m_time->release();
	m_input->release();
	d3d->release();
	mesh.release();
	material.release();
#pragma endregion

	return 0;
}