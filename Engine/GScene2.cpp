#include "GScene2.h"

void GScene2::Init()
{
	GetInstances();

#pragma region //Setup Light
	//Directional Light
	Lighting->DirectionalLight.direction = { -0.75f, -0.5f, 0.1f };
	Lighting->DirectionalLight.diffuse = { 1.0f, 1.0f, 0.9f, 1.0f };
	Lighting->DirectionalLight.ambient = { 0.2f, 0.2f, 0.25f, 1.0f };
	Lighting->DirectionalLight.intensity = 0.9f;
#pragma endregion

#pragma region //Setup Params
	Params->Parameters.diffuse = { 0.5f, 0.5f, 0.5f, 1 };
	Params->Parameters.roughness = sin(Time->GetTime());
#pragma endregion

#pragma region //Load Meshes
	CObjLoader obj = {};

	//Primitives
	m_skyBox.mesh.Init(obj.Load(EPrimitives::Cube));
	m_sphere.mesh.Init(obj.Load(L"R_Sphere.obj"));
#pragma endregion

#pragma region //Setup Materials
	m_mat_Sky.Init(L"S_SkyBox.hlsl");
	m_mat_Sky.SetColourTexture(L"T_SkyBox2.png");

	m_mat_Reflection.Init(L"S_Bubble.hlsl");
	m_mat_Reflection.SetColourTexture(L"T_SkySphere.png");
#pragma endregion

#pragma region //Pass Material to GameObject
	m_skyBox.material = &m_mat_Sky;
	m_sphere.material = &m_mat_Reflection;
#pragma endregion
}

void GScene2::Awake()
{
#pragma region //Setup Camera Transform
	Camera->SetPosition(0, 2, -5);
	Camera->SetRotation(90, 0);
#pragma endregion
}

void GScene2::Start()
{
#pragma region //Setup Contents Transforms
	m_skyBox.transform.SetRotation(0, 180, 0);
	m_skyBox.transform.SetScale(-1000);

	m_sphere.transform.SetRotation(-180, 0, 0);
#pragma endregion
}

void GScene2::EarlyUpdate()
{
#pragma region //Parent Camera to Skybox
	m_skyBox.transform.SetPosition(Camera->GetCamPosFloat3());
#pragma endregion
}

void GScene2::Update()
{
}

void GScene2::LateUpdate()
{
#pragma region //Render Contents
	m_skyBox.Update_Render();
	m_sphere.Update_Render();
#pragma endregion
}

void GScene2::Release()
{
	m_skyBox.Release();
	m_sphere.Release();
}
