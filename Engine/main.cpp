#include <windows.h>
#include "CWindow.h";
#include "CDirect.h";
#include "CMesh.h";
#include "CCamera.h";
#include "CTime.h";
#include "CLight.h";
#include "CMaterial.h";

INT WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, INT _nCmdShow)
{
	int error = 0;
	int width = 1000;
	int height = 550;
	bool windowed = true;

#pragma region //Init CoreFeatures
	CWindow window; //Create Window
	if (error = window.init(_hInstance, width, height, _nCmdShow) > 0) return error;

	CDirect d3d; //Create Direct3D 11
	if (error = d3d.init(window.getHWND(), window.getRECT(), windowed) > 0) return error;

	CMesh mesh; //Create Meshes
	if (error = mesh.init(d3d.getDevice()) > 0) return error;

	CCamera camera; //Create CameraView
	if (error = camera.init(window.getRECT()) > 0) return error;

	CTime* time; //Create singleton timer and deltaTime
	time = &time->getInstance();
	if (error = time->init() > 0) return error;

	CMaterial material; //Create Material for Mesh
	if (error = material.init(d3d.getDevice(), L"T_Main.jpeg") > 0) return error;

	CLight light = {};
	light.direction = { -1.0f, -1.0f, 1.0f };
	light.intensity = 0.6f;
	light.ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	light.diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
#pragma endregion

	//Start
	mesh.setPosition(0.0f, 0.0f, 0.0f);
	material.setLight(d3d.getDeviceContext(), light);

	//Update
	while (window.run()) 
	{
		d3d.clear();

		time->update();

		camera.update();

		mesh.update();
		material.render(d3d.getDeviceContext(), mesh.getWorldMatrix(), camera.getViewProjectionMatrix());
		mesh.render(d3d.getDeviceContext());

		d3d.getDeviceContext()->DrawIndexed(mesh.getIndexCount(), 0, 0);
		d3d.present();
	}

#pragma region //release
	window.release();
	d3d.release();
	mesh.release();
	camera.release();
	time->release();
	mesh.release();
	material.release();
#pragma endregion

	return 0;
}