#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct CDirectionalLight
{
	XMFLOAT3 direction;
	float intensity;
	XMFLOAT4 diffuse;
	XMFLOAT4 ambient;
};
struct CPointLight
{
	XMFLOAT3 position;
	float intensity;
	XMFLOAT4 diffuse;
	//float radius;
};

class CLighting
{
public:
	static CLighting& GetInstance() { static CLighting instance; return instance; }
	CLighting(CLighting const&) = delete;
	void operator=(CLighting const&) = delete;

	CDirectionalLight DirectionalLight;
	CPointLight PointLight;

private:
	CLighting() {}
};

