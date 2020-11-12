#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct CLight
{
	XMFLOAT3 direction;
	float intensity;
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
};

