#pragma once
#include <DirectXMath.h>
//#include <D3DX11.h>

using namespace DirectX;

struct SEnvironment 
{
	//LPCWSTR skyBox;
};
struct SParameters
{
	XMFLOAT4 diffuse;
	float roughness;
	float metallic;
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
	SEnvironment Environment;

private:
	CParameters() {}
};

