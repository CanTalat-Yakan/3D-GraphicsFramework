#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct SParameters
{
	XMFLOAT4 diffuse;
	float roughness;
	float metalic;
	float opacity;
	float pad;
};

class CParameters
{
public:
	static CParameters& GetInstance() { static CParameters instance; return instance; }
	CParameters(CParameters const&) = delete;
	void operator=(CParameters const&) = delete;

	SParameters Parameters;

private:
	CParameters() {}
};

