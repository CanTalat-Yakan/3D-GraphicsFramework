#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct CDirectionalLight
{
	XMFLOAT3 direction;
	float pad;
	XMFLOAT4 diffuse;
	XMFLOAT4 ambient;
	float intensity;
	XMFLOAT3 pad2;
};
struct CPointLight
{
	XMFLOAT3 position;
	float pad;
	XMFLOAT4 diffuse;
	float intensity;
	float radius;
	XMFLOAT2 pad2;
};

class CLighting
{
public:
	static CLighting& GetInstance() { static CLighting instance; return instance; }
	CLighting(CLighting const&) = delete;
	void operator=(CLighting const&) = delete;

	CDirectionalLight DirectionalLight;
	CPointLight PointLight, PointLight2, PointLight3, PointLight4;

private:
	CLighting() {}
};

