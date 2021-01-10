#include "GScene.h"
void GScene::Init()
{
	GetInstances();

#pragma region //Setup Light
	//Directional Light
	Lighting->DirectionalLight.direction = { -0.75f, -0.5f, 0.1f };
	Lighting->DirectionalLight.diffuse = { 1.0f, 1.0f, 0.9f, 1.0f };
	Lighting->DirectionalLight.ambient = { 0.2f, 0.2f, 0.25f, 1.0f };
	Lighting->DirectionalLight.intensity = 0.9f;

	//green Point Light
	Lighting->PointLight.diffuse = { 0.2f, 0.6f, 0.0f, 1.0f };
	Lighting->PointLight.position = { 0.0f, 0.5f, 0.0f };
	Lighting->PointLight.intensity = 0.7f;
	Lighting->PointLight.radius = 3.5f;
	//red Point Light
	Lighting->PointLight2.diffuse = { 0.6f, 0.2f, 0.0f, 1.0f };
	Lighting->PointLight2.position = { 3, 0.25f, 0.5f };
	Lighting->PointLight2.intensity = 0.7f;
	Lighting->PointLight2.radius = 5.5f;
	//blue Point Light
	Lighting->PointLight3.diffuse = { 0.0f, 0.2f, 0.6f, 1.0f };
	Lighting->PointLight3.position = { 0.25f, 0.2f, 2.5f };
	Lighting->PointLight3.intensity = 0.9f;
	Lighting->PointLight3.radius = 3.5f;
	//white Point Light
	Lighting->PointLight4.diffuse = { 0.9f, 0.9f, 0.9f, 1.0f };
	Lighting->PointLight4.position = { -3.0f, 2.3f, -1.0f };
	Lighting->PointLight4.intensity = 1.0f;
	Lighting->PointLight4.radius = 5.5f;
#pragma endregion

#pragma region //Load Meshes
	CObjLoader obj = {};

	//Primitives
	m_skyBox.mesh.Init(obj.Load(EPrimitives::Cube));
	m_plane.mesh.Init(obj.Load(EPrimitives::Plane));

	//Obj Files
	CObj cube = obj.Load(L"R_Cube.obj");
	m_cube.mesh.Init(cube);

	CObj sphere = obj.Load(L"R_Sphere.obj");
	m_sphere.mesh.Init(sphere);
	m_sphere2.mesh.Init(sphere);
	m_sphere3.mesh.Init(sphere);

	CObj cylinder = obj.Load(L"R_Cylinder_Hollow.obj");
	m_cylinder.mesh.Init(cylinder);
	m_cylinder2.mesh.Init(cylinder);
	m_cylinder3.mesh.Init(cylinder);

	CObj duck = obj.Load(L"R_Duck.obj");
	m_duck.mesh.Init(duck);
	m_duck2.mesh.Init(duck);
	m_duck3.mesh.Init(duck);

	CObj volcano = obj.Load(L"R_Volcano.obj");
	m_volcano.mesh.Init(volcano);

	CObj terrain = obj.LoadTerrain(300, 300);
	m_water.mesh.Init(terrain);
#pragma endregion

#pragma region //Setup Materials
	m_mat_Sky.Init(
		L"S_SkyBox.hlsl",
		L"T_SkyBox.png");

	m_mat_Standard.Init(
		L"S_Standard.hlsl",
		L"T_Gray.png");
	m_mat_Standard2.Init(
		L"S_Standard.hlsl",
		L"T_Proto2.png");
	m_mat_Standard3.Init(
		L"S_Standard.hlsl",
		L"T_Proto4.png",
		L"T_NormalDebug2.png");

	m_mat_Toon.Init(
		L"S_Toon.hlsl",
		L"T_Proto4.png");

	m_mat_Fresnel.Init(
		L"S_Fresnel.hlsl",
		L"T_Grid.png");

	m_mat_Duck.Init(
		L"S_Standard.hlsl",
		L"T_Duck.png");

	m_mat_Volcano.Init(
		L"S_Terrain.hlsl",
		L"T_Ground.png");

	m_mat_Water.Init(
		L"S_Water.hlsl",
		L"T_Grid.png");

	m_mat_Terrain.Init(
		L"S_Terrain.hlsl",
		L"T_Grid.png",
		L"T_Height.png");
#pragma endregion


#pragma region //Pass Material to GameObject
	m_skyBox.material = &m_mat_Sky;

	m_plane.material = &m_mat_Standard2;

	m_cube.material = &m_mat_Standard3;

	m_sphere.material = &m_mat_Standard3;
	m_sphere2.material = &m_mat_Toon;

	m_cylinder.material = &m_mat_Standard3;
	m_cylinder2.material = &m_mat_Toon;

	m_duck.material = &m_mat_Duck;
	m_duck2.material = &m_mat_Toon;

	m_volcano.material = &m_mat_Volcano;
	m_water.material = &m_mat_Water;


	m_sphere3.material = &m_mat_Fresnel;
	m_cylinder3.material = &m_mat_Fresnel;
	m_duck3.material = &m_mat_Fresnel;
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

	m_sphere.transform.SetRotation(-180, 0, 0);
	m_sphere.transform.SetScale(1.5f);
	//m_sphere.transform.SetScale(0.25f);
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

	m_duck.transform.SetRotation(90, 120, 10);
	m_duck.transform.SetScale(0.1f);
	m_duck.transform.SetPosition(-4.25f, 0, 0);
	m_duck2.transform = m_duck3.transform = m_duck.transform;
	m_duck2.transform.m_position.x += 3;
	m_duck2.transform.m_position.z += 3;
	m_duck3.transform.m_position.x += 3;
	m_duck3.transform.m_position.z += 6;

	m_volcano.transform.SetScale(0.0001, 0.0001, 0.0001);
	m_volcano.transform.SetRotation(0, 0, 180);
	m_volcano.transform.SetPosition(0, -10, 0);

	m_water.transform.SetPosition(-25, -8, -25);
	m_water.transform.SetScale(0.5f);
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

	m_cylinder.transform.SetRotationDeg(0, rot, 0);
	m_cylinder2.transform.SetRotationDeg(0, rot, 0);
	m_cylinder3.transform.SetRotationDeg(0, rot, 0);

	m_sphere.transform.SetRotationDeg(0, rot, 0);
	m_sphere2.transform.SetRotationDeg(0, rot, 0);
	m_sphere3.transform.SetRotationDeg(0, rot, 0);

	CTransform t;
	t.SetPosition(rot * Time->getDeltaTime(), 1, rot + 0.1f * Time->getDeltaTime());
	Lighting->PointLight3.position = t.m_position;
	//m_terrain.transform.SetRotationDeg(0, 0, rot);
#pragma endregion
}

void GScene::LateUpdate()
{
#pragma region //Render Contents
	m_skyBox.Update_Render();

	m_plane.Update_Render();

	m_cube.Update_Render();

	m_sphere.Update_Render();
	m_sphere2.Update_Render();
	m_sphere3.Update_Render();

	m_cylinder.Update_Render();
	m_cylinder2.Update_Render();
	m_cylinder3.Update_Render();

	m_duck.Update_Render();
	m_duck2.Update_Render();
	m_duck3.Update_Render();

	m_volcano.Update_Render();
	m_water.Update_Render();
#pragma endregion
}


void GScene::Release()
{
	m_skyBox.Release();
	m_plane.Release();
	m_cube.Release();
	m_sphere.Release();
	m_sphere2.Release();
	m_sphere3.Release();
	m_cylinder.Release();
	m_cylinder2.Release();
	m_cylinder3.Release();
	m_duck.Release();
	m_duck2.Release();
	m_duck3.Release();
	m_volcano.Release();
	m_water.Release();

	m_mat_Sky.Release();
	m_mat_Standard.Release();
	m_mat_Standard2.Release();
	m_mat_Standard3.Release();
	m_mat_Toon.Release();
	m_mat_Fresnel.Release();
	m_mat_Duck.Release();
}