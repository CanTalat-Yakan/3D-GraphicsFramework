#include <windows.h>
#include "CWindow.h";
#include "CDirect.h";
#include "CMesh.h";
#include "CCamera.h";
#include "CTime.h";
#include "CLight.h";
#include "CMaterial.h";
#include "CInput.h";

INT WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, INT _nCmdShow)
{
#pragma region //Set Variables
	int error = 0;
	int width = 1000;
	int height = 550;
	bool windowed = true;
#pragma endregion

#pragma region //Init CoreFeatures
	CWindow window; //Create Window
	if (error = window.init(_hInstance, width, height, _nCmdShow) > 0) return error;

	CTime* time; //Create singleton DeltaTime and Timer
	time = &time->getInstance();
	if (error = time->init() > 0) return error;

	CInput* input; //Create singleton Input
	input = &input->getInstance();
	if (error = input->init(_hInstance, window.getHWND()) > 0) return error;

	CCamera camera; //Create CameraView
	if (error = camera.init(window.getRECT()) > 0) return error;

	CDirect d3d; //Create Direct3D 11
	if (error = d3d.init(window.getHWND(), window.getRECT(), windowed) > 0) return error;

	CMesh mesh; //Create Meshes
	if (error = mesh.init(d3d.getDevice()) > 0) return error;

	CMaterial material; //Create Material for Mesh
	if (error = material.init(d3d.getDevice(), L"T_Main.jpeg") > 0) return error;

	CLight light = {};
	light.direction = { -0.7f, -0.5f, 1.0f };
	light.ambient = { 0.1f, 0.1f, 0.1f, 1.0f };
	light.diffuse = { 1.0f, 0.9f, 0.9f, 1.0f };
	light.intensity = 0.7f;
#pragma endregion

#pragma region //Start
	mesh.setPosition(0.0f, 0.0f, 0.0f);
	material.setLight(d3d.getDeviceContext(), light);
#pragma endregion
#pragma region //Update
	while (window.run()) 
	{
		d3d.clear();

		input->update();
		time->update();
		camera.update(window.getRECT());

		mesh.update();
		material.render(d3d.getDeviceContext(), mesh.getWorldMatrix(), camera.getViewProjectionMatrix());
		mesh.render(d3d.getDeviceContext());

		d3d.getDeviceContext()->DrawIndexed(mesh.getIndexCount(), 0, 0);
		d3d.present();
	}
#pragma endregion

#pragma region //release
	window.release();
	time->release();
	input->release();
	camera.release();
	d3d.release();
	mesh.release();
	material.release();
#pragma endregion

	return 0;
}