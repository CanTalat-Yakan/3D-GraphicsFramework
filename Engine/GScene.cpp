#include "GScene.h"
void GScene::Init()
{
	d3d = &d3d->GetInstance();
	camera = &camera->GetInstance();
	time = &time->GetInstance();
}

void GScene::Awake()
{
	m_skyBox.Init(m_obj.Load(EPrimitives::Cube));
	m_plane.Init(m_obj.Load(EPrimitives::Cube));
	m_cube.Init(m_obj.Load(EPrimitives::Cube));
	m_cube2.Init(m_obj.Load(L"R_Cube.obj"));

	m_material00.Init(L"S_SkyBox.hlsl", L"T_SkyBox2.png");
	m_material01.Init(L"S_Standard.hlsl", L"T_Proto.png");
	m_material02.Init(L"S_Standard.hlsl", L"T_Main.jpeg");

	m_light.direction = { -0.7f, -0.5f, 0.5f };
	m_light.ambient = { 0.1f, 0.1f, 0.1f, 1.0f };
	m_light.diffuse = { 1.0f, 0.9f, 0.9f, 1.0f };
	m_light.intensity = 1.1f;
}

void GScene::Start()
{
	m_skyBox.SetRotation(0, XMConvertToRadians(180), 0);
	m_skyBox.SetScale(-1000, -1000, -1000);

	m_plane.SetScale(10, 0.1f, 10);
	m_plane.SetPosition(0, -1.5f, 0);

	m_cube2.SetPosition(0, 2, 0);

	m_material01.SetLightBuffer(m_light);
	m_material02.SetLightBuffer(m_light);
}

void GScene::Update()
{
	m_skyBox.SetPosition(camera->getCamPosFloat3());

	float rot = time->getDeltaTime() * 0.1f;
	m_cube.SetRotation(rot);
}

void GScene::LateUpdate()
{
	m_material00.Render(m_skyBox.GetWorldMatrix());
	m_skyBox.Render();
	m_skyBox.Update();

	m_material01.Render(m_plane.GetWorldMatrix());
	m_plane.Render();
	m_plane.Update();

	m_material02.Render(m_cube.GetWorldMatrix());
	m_cube.Render();
	m_cube.Update();

	m_material02.Render(m_cube2.GetWorldMatrix());
	m_cube2.Render();
	m_cube2.Update();
}

void GScene::Release()
{
	m_skyBox.Release();
	m_plane.Release();
	m_cube.Release();
	m_cube2.Release();
	m_material00.Release();
	m_material01.Release();
	m_material02.Release();
}