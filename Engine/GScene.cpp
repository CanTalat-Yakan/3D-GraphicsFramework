#include "GScene.h"
void GScene::Init()
{
	GetInstances();

#pragma region //Load Meshes
	CObjLoader m_obj = {};

	//Primitives
	m_skyBox.mesh.Init(m_obj.Load(EPrimitives::Cube));
	m_plane.mesh.Init(m_obj.Load(EPrimitives::Plane));

	//Obj Files
	CObj cube = m_obj.Load(L"R_Cube.obj");
	m_cube.mesh.Init(cube);
	m_cube2.mesh.Init(cube);
	m_cube3.mesh.Init(cube);

	CObj sphere = m_obj.Load(L"R_Sphere.obj");
	m_sphere.mesh.Init(sphere);
	m_sphere2.mesh.Init(sphere);
	m_sphere3.mesh.Init(sphere);

	CObj cylinder = m_obj.Load(L"R_Cylinder_Hollow.obj");
	m_cylinder.mesh.Init(cylinder);
	m_cylinder2.mesh.Init(cylinder);
	m_cylinder3.mesh.Init(cylinder);

	CObj bird = m_obj.Load(L"R_Bird.obj");
	m_bird.mesh.Init(bird);
	m_bird2.mesh.Init(bird);
	m_bird3.mesh.Init(bird);
#pragma endregion

#pragma region //Setup Materials
	m_mat_Sky.Init(
		L"S_SkyBox.hlsl", 
		L"T_SkyBox.png");
	m_mat_Standard.Init(
		L"S_Standard.hlsl", 
		L"T_White.png");
	m_mat_Standard2.Init(
		L"S_Standard.hlsl", 
		L"T_Grid.png", 
		L"T_NormalDebug2.png");
	m_mat_Standard3.Init(
		L"S_Standard.hlsl", 
		L"T_Proto.png");
	m_mat_Toon.Init(
		L"S_Toon.hlsl", 
		L"T_Grid.png");
	m_mat_Fresnel.Init(
		L"S_Fresnel.hlsl", 
		L"T_Grid.png");
	m_mat_Bird.Init(
		L"S_Standard.hlsl", 
		L"T_Bird.png");
#pragma endregion

#pragma region //Setup Light
	m_light.direction = { -0.75f, -0.5f, 0.1f };
	m_light.ambient = { 0.2f, 0.2f, 0.25f, 1.0f };
	m_light.diffuse = { 1.0f, 1.0f, 0.9f, 1.0f };
	m_light.intensity = 0.9f;
#pragma endregion

#pragma region //Assign Light to Materials
	m_mat_Standard.SetPerFrameBuffer(m_light);
#pragma endregion
}

void GScene::Awake()
{
#pragma region //Setup Camera Transform
	Camera->SetPosition(0, 2, -5);
	Camera->SetRotation(90, 0);
#pragma endregion
}

void GScene::Start()
{
#pragma region //Setup Contents Transforms
	m_skyBox.transform.SetRotation(0, 180, 0);
	m_skyBox.transform.SetScale(-1000);

	m_plane.transform.SetPosition(0, 0, 2);

	m_cube.transform.SetPosition(0, 1.5f, 0);
	m_cube2.transform.SetPosition(0, 1.5f, 2);
	m_cube3.transform.SetPosition(0, 1.5f, 4);

	m_sphere.transform.SetRotation(-180, 0, 0);
	m_sphere.transform.SetScale(1.5f);
	m_sphere.transform.SetPosition(2, 1.5f, 0);
	m_sphere2.transform = m_sphere3.transform = m_sphere.transform;
	m_sphere2.transform.m_position.z += 2;
	m_sphere3.transform.m_position.z += 4;

	m_cylinder.transform.SetRotation(90, 0, 0);
	m_cylinder.transform.SetScale(0.3f);
	m_cylinder.transform.SetPosition(4, 1, 0);
	m_cylinder2.transform = m_cylinder3.transform = m_cylinder.transform;
	m_cylinder2.transform.m_position.z += 2;
	m_cylinder3.transform.m_position.z += 4;

	m_bird.transform.SetRotation(90, 120, 10);
	m_bird.transform.SetScale(0.1f);
	m_bird.transform.SetPosition(-4.25f, 0, 0);
	m_bird2.transform = m_bird3.transform = m_bird.transform;
	m_bird2.transform.m_position.x += 1;
	m_bird2.transform.m_position.z += 3;
	m_bird3.transform.m_position.x += 1;
	m_bird3.transform.m_position.z += 6;
#pragma endregion
}

void GScene::EarlyUpdate()
{
#pragma region //Parent Camera to Skybox
	m_skyBox.transform.SetPosition(Camera->GetCamPosFloat3());
#pragma endregion
}

void GScene::Update()
{
#pragma region //Update Contents Transforms
	float rot = Time->getDeltaTime();

	m_cube.transform.SetRotationDeg(0, rot, 0);
	m_cube2.transform.SetRotationDeg(0, rot, 0);
	m_cube3.transform.SetRotationDeg(0, rot, 0);

	m_cylinder.transform.SetRotationDeg(0, rot, 0);
	m_cylinder2.transform.SetRotationDeg(0, rot, 0);
	m_cylinder3.transform.SetRotationDeg(0, rot, 0);

	m_sphere.transform.SetRotationDeg(0, rot, 0);
	m_sphere2.transform.SetRotationDeg(0, rot, 0);
	m_sphere3.transform.SetRotationDeg(0, rot, 0);
#pragma endregion
}

void GScene::LateUpdate()
{
#pragma region //Render Contents
	m_skyBox.Update_Render(m_mat_Sky);

	m_plane.Update_Render(m_mat_Standard3);

	m_cube.Update_Render(m_mat_Standard2);
	m_cube2.Update_Render(m_mat_Toon);
	m_cube3.Update_Render(m_mat_Fresnel);

	m_sphere.Update_Render(m_mat_Standard2);
	m_sphere2.Update_Render(m_mat_Toon);
	m_sphere3.Update_Render(m_mat_Fresnel);

	m_cylinder.Update_Render(m_mat_Standard2);
	m_cylinder2.Update_Render(m_mat_Toon);
	m_cylinder3.Update_Render(m_mat_Fresnel);

	m_bird.Update_Render(m_mat_Bird);
	m_bird2.Update_Render(m_mat_Toon);
	m_bird3.Update_Render(m_mat_Fresnel);
#pragma endregion
}

void GScene::Release()
{
	m_skyBox.Release();
	m_plane.Release();
	m_cube.Release();
	m_cube2.Release();
	m_cube3.Release();
	m_sphere.Release();
	m_sphere2.Release();
	m_sphere3.Release();
	m_cylinder.Release();
	m_cylinder2.Release();
	m_cylinder3.Release();
	m_bird.Release();
	m_bird2.Release();
	m_bird3.Release();

	m_mat_Sky.Release();
	m_mat_Standard.Release();
	m_mat_Standard2.Release();
	m_mat_Standard3.Release();
	m_mat_Toon.Release();
	m_mat_Fresnel.Release();
	m_mat_Bird.Release();
}