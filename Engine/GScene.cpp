#include "GScene.h"
void GScene::Init()
{
	d3d = &d3d->GetInstance();
	camera = &camera->GetInstance();
	time = &time->GetInstance();

	m_skyBox.Init(m_obj.Load(EPrimitives::Cube));
	m_plane.Init(m_obj.Load(EPrimitives::Plane));
	m_cube.Init(m_obj.Load(L"R_Cube.obj"));
	m_cylinder.Init(m_obj.Load(L"R_Cylinder_Hollow.obj"));
	CObj sphere = m_obj.Load(L"R_Sphere.obj");
	m_sphere.Init(sphere);
	m_sphere2.Init(sphere);
	m_sphere3.Init(sphere);
	m_sphere4.Init(sphere);
	m_bird.Init(m_obj.Load(L"R_Bird.obj"));

	m_material_Sky.Init(L"S_SkyBox.hlsl", L"T_SkyBox.png");
	m_material_Proto.Init(L"S_Standard.hlsl", L"T_Proto.png");
	m_material_Cells.Init(L"S_Standard.hlsl", L"T_Cell.jpeg");
	m_material_CellShader.Init(L"S_Cell.hlsl", L"T_Cell.jpeg");
	m_material_Fresnel.Init(L"S_Fresnel.hlsl", L"T_Cell.jpeg");
	m_material_Bird.Init(L"S_Standard.hlsl", L"T_Bird.jpg");
}

void GScene::Awake()
{
	m_light.direction = { -0.7f, -0.5f, 0.5f };
	m_light.ambient = { 0.1f, 0.1f, 0.1f, 1.0f };
	m_light.diffuse = { 1.0f, 0.9f, 0.9f, 1.0f };
	m_light.intensity = 1.1f;

	m_material_Proto.SetLightBuffer(m_light);
	m_material_Cells.SetLightBuffer(m_light);
	m_material_Fresnel.SetLightBuffer(m_light);
	m_material_Bird.SetLightBuffer(m_light);

	camera->SetPosition(0, 2, -5);
}

void GScene::Start()
{
	m_skyBox.SetRotation(0, 180, 0);
	m_skyBox.SetScale(-1000);

	m_cube.SetPosition(0, 1.5f, 0);

	m_cylinder.SetRotation(90, 0, 0);
	m_cylinder.SetScale(0.3f);
	m_cylinder.SetPosition(2, 1, 0);

	m_sphere.SetRotation(-180, 0, 0);
	m_sphere.SetScale(1.5f);
	m_sphere.SetPosition(-2, 1.5f, 0);

	m_sphere2.SetRotation(-180, 0, 0);
	m_sphere2.SetScale(1.5f);
	m_sphere2.SetPosition(-2, 1.5f, 2);

	m_sphere3.SetRotation(-180, 0, 0);
	m_sphere3.SetScale(1.5f);
	m_sphere3.SetPosition(-2, 1.5f, -2);

	m_sphere4.SetRotation(-180, 0, 0);
	m_sphere4.SetScale(1.5f);
	m_sphere4.SetPosition(-2, 1.5f, -4);

	m_bird.SetScale(0.1f, 0.1f, 0.1f);
	m_bird.SetRotation(90, 120, 10);
	m_bird.SetPosition(-5, 0, 0);
}

void GScene::Update()
{
	m_skyBox.SetPosition(camera->GetCamPosFloat3());

	float rot = time->getDeltaTime();
	m_cylinder.SetRotationDeg(0, rot, 0);
	m_sphere.SetRotationDeg(0, rot, 0);
	m_sphere2.SetRotationDeg(0, rot, 0);
	m_sphere3.SetRotationDeg(0, rot, 0);
	m_sphere4.SetRotationDeg(0, rot, 0);
}

void GScene::LateUpdate()
{
	m_material_Sky.Render(m_skyBox.GetWorldMatrix());
	m_skyBox.Render();
	m_skyBox.Update();

	m_material_Proto.Render(m_plane.GetWorldMatrix());
	m_plane.Render();
	m_plane.Update();

	m_material_Cells.Render(m_cylinder.GetWorldMatrix());
	m_cylinder.Render();
	m_cylinder.Update();

	m_material_Cells.Render(m_sphere.GetWorldMatrix());
	m_sphere.Render();
	m_sphere.Update();

	m_material_Fresnel.Render(m_sphere2.GetWorldMatrix());
	m_sphere2.Render();
	m_sphere2.Update();

	m_material_CellShader.Render(m_sphere3.GetWorldMatrix());
	m_sphere3.Render();
	m_sphere3.Update();

	m_material_Cells.Render(m_sphere4.GetWorldMatrix());
	m_sphere4.Render();
	m_sphere4.Update();

	m_material_Cells.Render(m_cube.GetWorldMatrix());
	m_cube.Render();
	m_cube.Update();

	m_material_Bird.Render(m_bird.GetWorldMatrix());
	m_bird.Render();
	m_bird.Update();
}

void GScene::Release()
{
	m_skyBox.Release();
	m_plane.Release();
	m_cube.Release();
	m_cylinder.Release();
	m_sphere.Release();
	m_sphere2.Release();
	m_bird.Release();

	m_material_Sky.Release();
	m_material_Proto.Release();
	m_material_Cells.Release();
}