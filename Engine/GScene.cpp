#include "GScene.h"
void GScene::Init()
{
#pragma region //Get Instances of DirectX, Camera and Time
	d3d = &d3d->GetInstance();
	camera = &camera->GetInstance();
	time = &time->GetInstance();
#pragma endregion

#pragma region //Load Meshes
	//Primitives
	m_skyBox.Init(m_obj.Load(EPrimitives::Cube));
	m_plane.Init(m_obj.Load(EPrimitives::Plane));

	//Obj Files
	CObj cube = m_obj.Load(L"R_Cube.obj");
	m_cube.Init(cube);

	CObj cylinder = m_obj.Load(L"R_Cylinder_Hollow.obj");
	m_cylinder.Init(cylinder);

	CObj sphere = m_obj.Load(L"R_Sphere.obj");
	m_sphere.Init(sphere);
	m_sphere2.Init(sphere);
	m_sphere3.Init(sphere);

	CObj bird = m_obj.Load(L"R_Bird.obj");
	m_bird.Init(bird);
#pragma endregion

#pragma region //Setup Materials
	m_material_Sky.Init(L"S_SkyBox.hlsl", L"T_SkyBox2.png");
	m_material_Standard.Init(L"S_Standard.hlsl", L"T_Blue.png");
	m_material_Proto.Init(L"S_Standard.hlsl", L"T_Proto.png");
	m_material_Cells.Init(L"S_Standard.hlsl", L"T_Cell.jpeg");
	m_material_CellShader.Init(L"S_Cell.hlsl", L"T_Proto.png");
	m_material_Fresnel.Init(L"S_Fresnel.hlsl", L"T_Proto.png");
	m_material_Bird.Init(L"S_Standard.hlsl", L"T_Bird.jpg");
#pragma endregion
}

void GScene::Awake()
{
#pragma region //Setup Light
	m_light.direction = { -0.75f, -0.5f, 0.4f };
	m_light.ambient = { 0.1f, 0.1f, 0.1f, 1.0f };
	m_light.diffuse = { 1.0f, 0.9f, 0.9f, 1.0f };
	m_light.intensity = 1.1f;
#pragma endregion

#pragma region //Assign Light to Materials
	m_material_Standard.SetLightBuffer(m_light);
#pragma endregion

#pragma region //Setup Camera Transform
	camera->SetPosition(0, 2, -5);
	camera->SetRotation(90, 0);
#pragma endregion
}

void GScene::Start()
{
#pragma region //Setup Contents Transforms
	m_skyBox.SetRotation(0, 180, 0);
	m_skyBox.SetScale(-1000);

	m_cube.SetPosition(0, 1.5f, 0);

	m_cylinder.SetRotation(90, 0, 0);
	m_cylinder.SetScale(0.3f);
	m_cylinder.SetPosition(2, 1, 0);

	m_sphere.SetRotation(-180, 0, 0);
	m_sphere.SetScale(1.5f);
	m_sphere.SetPosition(-2, 1.5f, 2);

	m_sphere2.SetRotation(-180, 0, 0);
	m_sphere2.SetScale(1.5f);
	m_sphere2.SetPosition(-2, 1.5f, 0);

	m_sphere3.SetRotation(-180, 0, 0);
	m_sphere3.SetScale(1.5f);
	m_sphere3.SetPosition(-2, 1.5f, -2);

	m_bird.SetScale(0.1f, 0.1f, 0.1f);
	m_bird.SetRotation(90, 120, 10);
	m_bird.SetPosition(-5, 0, 0);
#pragma endregion
}

void GScene::EarlyUpdate()
{
#pragma region //Parent Camera to Skybox
	m_skyBox.SetPosition(camera->GetCamPosFloat3());
#pragma endregion
}

void GScene::Update()
{
#pragma region //Update Contents Transforms
	float rot = time->getDeltaTime(); 

	m_cylinder.SetRotationDeg(0, rot, 0);

	m_sphere.SetRotationDeg(0, rot, 0);
	m_sphere2.SetRotationDeg(0, rot, 0);
	m_sphere3.SetRotationDeg(0, rot, 0);
#pragma endregion
}

void GScene::LateUpdate()
{
#pragma region //Render Contents
	m_skyBox.Update_Render(m_material_Sky);

	m_plane.Update_Render(m_material_Proto);

	m_cube.Update_Render(m_material_Cells);

	m_cylinder.Update_Render(m_material_Cells);

	m_sphere.Update_Render(m_material_Proto);
	m_sphere2.Update_Render(m_material_Fresnel);
	m_sphere3.Update_Render(m_material_CellShader);

	m_bird.Update_Render(m_material_Bird);
#pragma endregion
}

void GScene::Release()
{
	m_skyBox.Release();
	m_plane.Release();
	m_cube.Release();
	m_cylinder.Release();
	m_sphere.Release();
	m_sphere2.Release();
	m_sphere3.Release();
	m_bird.Release();

	m_material_Sky.Release();
	m_material_Proto.Release();
	m_material_Cells.Release();
	m_material_CellShader.Release();
	m_material_Fresnel.Release();
	m_material_Bird.Release();
}