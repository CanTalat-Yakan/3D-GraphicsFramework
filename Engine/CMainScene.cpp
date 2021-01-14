#include "CScene.h"

void CScene::Init()
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
	Lighting->PointLight2.radius = 4.5f;
	//blue Point Light
	Lighting->PointLight3.diffuse = { 0.0f, 0.2f, 0.6f, 1.0f };
	Lighting->PointLight3.position = { 0.25f, 0.2f, 2.5f };
	Lighting->PointLight3.intensity = 2.9f;
	Lighting->PointLight3.radius = 3.5f;
	//white Point Light
	Lighting->PointLight4.diffuse = { 0.9f, 0.9f, 0.9f, 1.0f };
	Lighting->PointLight4.position = { -2.65f, 2.3f, -1.0f };
	Lighting->PointLight4.intensity = 1.0f;
	Lighting->PointLight4.radius = 2.5f;
#pragma endregion

#pragma region //Setup Params Default
	Params->Parameters.diffuse = { 0.5f, 0.5f, 0.5f, 1 };
	Params->Parameters.roughness = 0.5f;
	Params->Parameters.metallic = 0.5f;
	Params->Parameters.opacity = 0;

	//Params->Environment.skyBox = L"T_SkyBox.png";
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
	m_sphere4.mesh.Init(sphere);
	m_sphere5.mesh.Init(sphere);
	m_sphere6.mesh.Init(sphere);
	m_sphere7.mesh.Init(sphere);
	m_sphere8.mesh.Init(sphere);
	m_sphere9.mesh.Init(sphere);
	m_pointLightVisualizer.mesh.Init(sphere);

	CObj cylinder = obj.Load(L"R_Cylinder_Hollow.obj");
	m_cylinder.mesh.Init(cylinder);
	m_cylinder2.mesh.Init(cylinder);
	m_cylinder3.mesh.Init(cylinder);

	CObj duck = obj.Load(L"R_Duck.obj");
	m_duck.mesh.Init(duck);
	m_duck2.mesh.Init(duck);
	m_duck3.mesh.Init(duck);

	CObj terrain = obj.LoadTerrain(200, 200);
	m_water.mesh.Init(terrain);
	m_terrain.mesh.Init(terrain);
#pragma endregion

#pragma region //Setup Materials
	m_mat_Sky.Init(L"S_SkyBox.hlsl");
	m_mat_Sky.SetSkyBoxTexture(L"T_SkyBox2.png");

	m_mat_Plane.Init(L"S_Standard.hlsl");
	m_mat_Plane.SetColourTexture(L"T_Proto2.png");

	m_mat_Grid.Init(L"S_Standard.hlsl");
	m_mat_Grid.SetColourTexture(L"T_Proto4.png");
	m_mat_Grid.SetSkyBoxTexture(L"T_Grid.png");

	m_mat_Normal.withGS = true;
	m_mat_Normal.Init(L"S_Standard.hlsl");
	m_mat_Normal.SetColourTexture(L"T_Proto4.png");
	m_mat_Normal.SetNormalTexture(L"T_NormalDebug2.png");

	m_mat_Toon.Init(L"S_Toon.hlsl");
	m_mat_Toon.SetColourTexture(L"T_Proto4.png");

	m_mat_Bubble.Init(L"S_Bubble.hlsl");
	m_mat_Bubble.SetSkyBoxTexture(L"T_Grid.png");

	m_mat_Duck.Init(L"S_Standard.hlsl");
	m_mat_Duck.SetColourTexture(L"T_Duck.png");

	m_mat_Terrain.withGS = true;
	m_mat_Terrain.Init(L"S_Terrain.hlsl");
	m_mat_Terrain.SetHeightTexture(L"T_Height.png");
	m_mat_Terrain.SetColourTexture(L"T_Ground.png");

	m_mat_Water.withGS = true;
	m_mat_Water.Init(L"S_Water.hlsl");
	m_mat_Water.SetHeightTexture(L"T_Height.png");
	m_mat_Water.SetColourTexture(L"T_Water.png");

	m_mat_Unlit.Init(L"S_Unlit.hlsl");
	m_mat_Unlit.SetColourTexture(L"T_Grid.png");
#pragma endregion

#pragma region //Pass Material to GameObject
	m_skyBox.material = &m_mat_Sky;

	m_cube.material = &m_mat_Normal;

	m_plane.material = &m_mat_Grid;
	m_sphere.material = &m_mat_Grid;
	m_sphere4.material = &m_mat_Grid;
	m_sphere5.material = &m_mat_Grid;
	m_sphere6.material = &m_mat_Grid;
	m_sphere7.material = &m_mat_Grid;
	m_sphere8.material = &m_mat_Grid;
	m_sphere9.material = &m_mat_Grid;
	m_cylinder.material = &m_mat_Grid;

	m_sphere2.material = &m_mat_Toon;
	m_cylinder2.material = &m_mat_Toon;
	m_duck2.material = &m_mat_Toon;

	m_duck.material = &m_mat_Duck;

	m_water.material = &m_mat_Water;

	m_terrain.material = &m_mat_Terrain;

	m_sphere3.material = &m_mat_Bubble;
	m_cylinder3.material = &m_mat_Bubble;
	m_duck3.material = &m_mat_Bubble;

	m_pointLightVisualizer.material = &m_mat_Unlit;
#pragma endregion
}


void CScene::Awake()
{
#pragma region //Setup Camera Transform
	Camera->SetPosition(0, 2, -5);
	Camera->SetRotation(90, 0);
#pragma endregion
}

void CScene::Start()
{
#pragma region //Setup Contents Transforms
	m_skyBox.transform.SetRotation(0, 180, 0);
	m_skyBox.transform.SetScale(-1000);

	m_pointLightVisualizer.transform.SetScale(0.1f);

	m_plane.transform.SetPosition(0, 0, 2);

	m_cube.transform.SetPosition(0, 1.5f, 0);

	m_sphere.transform.SetRotation(-180, 0, 0);
	m_sphere.transform.SetScale(1.5f);
	//m_sphere.transform.SetScale(0.25f);
	m_sphere.transform.SetPosition(2, 1.5f, 0);
	m_sphere2.transform = m_sphere3.transform = m_sphere4.transform = 
		m_sphere5.transform = m_sphere6.transform = m_sphere7.transform = 
		m_sphere8.transform = m_sphere9.transform = m_sphere.transform;
	m_sphere2.transform.m_position.z += 2;
	m_sphere3.transform.m_position.z += 4;

	m_sphere4.transform.m_position.z -= 2;
	m_sphere4.transform.m_position.x -= 2;
	m_sphere7.transform.m_position.z -= 2;
	m_sphere7.transform.m_position.x -= 2;
	m_sphere7.transform.m_position.y += 2;

	m_sphere5.transform.m_position.z -= 2;
	m_sphere8.transform.m_position.z -= 2;
	m_sphere8.transform.m_position.y += 2;

	m_sphere6.transform.m_position.x += 2;
	m_sphere6.transform.m_position.z -= 2;
	m_sphere9.transform.m_position.z -= 2;
	m_sphere9.transform.m_position.x += 2;
	m_sphere9.transform.m_position.y += 2;


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

	m_water.transform.SetPosition(-100, -13, -100);

	m_terrain.transform.SetPosition(-100, -15, -100);
#pragma endregion
}


void CScene::EarlyUpdate()
{
#pragma region //Parent Camera to Skybox
	m_skyBox.transform.SetPosition(Camera->GetCamPosFloat3());
#pragma endregion
}

void CScene::Update()
{
#pragma region //Update Contents Transforms
	float rot = Time->GetDeltaTime();

	m_cube.transform.SetRotationDegAdditive(0, rot, 0);

	m_cylinder.transform.SetRotationDegAdditive(0, rot, 0);
	m_cylinder2.transform.SetRotationDegAdditive(0, rot, 0);
	m_cylinder3.transform.SetRotationDegAdditive(0, rot, 0);

	m_sphere.transform.SetRotationDegAdditive(0, rot, 0);
	m_sphere2.transform.m_rotation = m_sphere3.transform.m_rotation = m_sphere4.transform.m_rotation = 
		m_sphere5.transform.m_rotation = m_sphere6.transform.m_rotation = m_sphere7.transform.m_rotation = 
		m_sphere8.transform.m_rotation = m_sphere9.transform.m_rotation = m_sphere.transform.m_rotation;

	CTransform t;
	t.SetPosition(sin(Time->GetTime()) * 2, sin(Time->GetTime() * 2) + 1.1f, cos(Time->GetTime()) * 2);
	Lighting->PointLight3.position = t.m_position;

	t.SetPosition(-2.65f, sin(Time->GetTime() * 2) + 1.2f, -1.0f);
	Lighting->PointLight4.position = t.m_position;
#pragma endregion
}

void CScene::LateUpdate()
{
#pragma region //Visualize PointLights
	m_pointLightVisualizer.transform.SetPosition(Lighting->PointLight.position);
	Params->Parameters.diffuse = { Lighting->PointLight.diffuse };
	m_pointLightVisualizer.Update_Render();
	m_pointLightVisualizer.transform.SetPosition(Lighting->PointLight2.position);
	Params->Parameters.diffuse = { Lighting->PointLight2.diffuse };
	m_pointLightVisualizer.Update_Render();
	m_pointLightVisualizer.transform.SetPosition(Lighting->PointLight3.position);
	Params->Parameters.diffuse = { Lighting->PointLight3.diffuse };
	m_pointLightVisualizer.Update_Render();
	m_pointLightVisualizer.transform.SetPosition(Lighting->PointLight4.position);
	Params->Parameters.diffuse = { Lighting->PointLight4.diffuse };
	m_pointLightVisualizer.Update_Render();
#pragma endregion

#pragma region //Render Contents
	Params->Parameters.diffuse = { 1, 1, 1, 1 };
	Params->Parameters.roughness = 0;
	Params->Parameters.metallic = 0;
	m_skyBox.Update_Render();

	m_plane.Update_Render();

	m_cube.Update_Render();

	m_sphere.Update_Render();
	m_sphere2.Update_Render();
	m_sphere3.Update_Render();
	Params->Parameters.roughness = 1;
	m_sphere7.Update_Render();
	Params->Parameters.roughness = 0;
	Params->Parameters.metallic = 1;
	m_sphere8.Update_Render();
	Params->Parameters.roughness = 1;
	m_sphere9.Update_Render();
	Params->Parameters.metallic = 0;
	Params->Parameters.roughness = 0.5f + 0.5f * sin(Time->GetTime() * 2);
	m_sphere4.Update_Render();
	Params->Parameters.metallic = 0.5f + 0.5f * sin(Time->GetTime() * 2);
	m_sphere6.Update_Render();
	Params->Parameters.roughness = 0;
	m_sphere5.Update_Render();
	Params->Parameters.metallic = 0;

	Params->Parameters.diffuse = { 1, 1, 1, 1 };

	m_cylinder.Update_Render();
	m_cylinder2.Update_Render();
	m_cylinder3.Update_Render();

	Params->Parameters.diffuse = { 0.5f, 0.5f, 0.5f, 1 };
	m_duck.Update_Render();
	m_duck2.Update_Render();
	m_duck3.Update_Render();

	Params->Parameters.roughness = 0.3;
	m_terrain.Update_Render();
	Params->Parameters.roughness = 0;
	Params->Parameters.metallic = 0.3;
	m_water.Update_Render();
	Params->Parameters.metallic = 0;
#pragma endregion
}


void CScene::Release()
{
	m_skyBox.Release();
	m_plane.Release();
	m_cube.Release();
	m_sphere.Release();
	m_sphere2.Release();
	m_sphere3.Release();
	m_sphere4.Release();
	m_sphere5.Release();
	m_sphere6.Release();
	m_sphere7.Release();
	m_sphere8.Release();
	m_sphere9.Release();
	m_cylinder.Release();
	m_cylinder2.Release();
	m_cylinder3.Release();
	m_duck.Release();
	m_duck2.Release();
	m_duck3.Release();
	m_terrain.Release();
	m_water.Release();
	m_pointLightVisualizer.Release();

	m_mat_Sky.Release();
	m_mat_Plane.Release();
	m_mat_Grid.Release();
	m_mat_Normal.Release();
	m_mat_Toon.Release();
	m_mat_Bubble.Release();
	m_mat_Duck.Release();
	m_mat_Water.Release();
	m_mat_Terrain.Release();
	m_mat_Unlit.Release();
}