#include "GScene.h"
void GScene::Init()
{
	d3d = &d3d->getInstance();
	camera = &camera->getInstance();
	time = &time->getInstance();
}

void GScene::Awake()
{
	//m_obj.load(L"R_Cube.obj")
	m_skyBox.Init(m_obj.load(EPrimitives::Cube));
	m_plane.Init(m_obj.load(EPrimitives::Cube));
	m_cube.Init(m_obj.load(EPrimitives::Cube));

	m_material00.Init(L"S_SkyBox.hlsl", L"T_SkyBox.png");
	m_material01.Init(L"S_Standard.hlsl", L"T_Proto.png");
	m_material02.Init(L"S_Standard.hlsl", L"T_Main.jpeg");

	m_light.direction = { -0.7f, -0.5f, 0.5f };
	m_light.ambient = { 0.1f, 0.1f, 0.1f, 1.0f };
	m_light.diffuse = { 1.0f, 0.9f, 0.9f, 1.0f };
	m_light.intensity = 0.9f;
}

void GScene::Start()
{
	m_skyBox.setScale(-1000, -1000, -1000);

	m_plane.setScale(10, 0.1f, 10);
	m_plane.setPosition(0, -1.5f, 0);

	m_material00.SetLightBuffer(m_light);
	m_material01.SetLightBuffer(m_light);
	m_material02.SetLightBuffer(m_light);
}

void GScene::Update()
{
	m_skyBox.setPosition(camera->getCamPosFloat3());

	float rotationspeed = time->getDeltaTime() * 0.1f;
	XMVECTOR quaternion = m_cube.getRotation();
	quaternion = XMQuaternionMultiply(quaternion, XMQuaternionRotationRollPitchYaw(rotationspeed, rotationspeed, rotationspeed));
	m_cube.setRotation(quaternion);
}

void GScene::LateUpdate()
{
	m_material00.Render(m_skyBox.getWorldMatrix());
	m_skyBox.Render();
	m_skyBox.Update();

	m_material01.Render(m_plane.getWorldMatrix());
	m_plane.Render();
	m_plane.Update();

	m_material02.Render(m_cube.getWorldMatrix());
	m_cube.Render();
	m_cube.Update();
}

void GScene::Release()
{
	m_skyBox.Release();
	m_plane.Release();
	m_cube.Release();
	m_material00.Release();
	m_material01.Release();
	m_material02.Release();
}