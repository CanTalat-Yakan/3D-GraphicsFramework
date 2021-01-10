#pragma once
#include "CLighting.h"
#include "CDirect.h"
#include "CCamera.h"
#include "CTime.h"
#include <d3d11.h>
#include <D3DX11.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3dx11.lib")

using namespace DirectX;

class CMaterial
{
public:
	int Init(LPCWSTR _shaderName, LPCWSTR _textureName);
	int Init(LPCWSTR _shaderName, LPCWSTR _textureName, LPCWSTR _normalTextureName);
	void Render(XMMATRIX _worldMatrix);
	void Release();


private:
	CDirect* p_d3d;
	CCamera* p_camera;
	CLighting* p_lighting;
	CTime* p_time;


	int createVertexShader(LPCWSTR _shaderName);
	int createPixelShader(LPCWSTR _shaderName);
	int createInputLayout(ID3DBlob* _pBlob);
	int createBuffer(ID3D11Buffer** _pcb, UINT _cbSize);
	int createTextureAndSampler(LPCWSTR _textureName, ID3D11ShaderResourceView** _texture_SRV);

	void setMatrixBuffer(XMMATRIX _worldMatrix);
	void SetLightingBuffer();
	void SetParameterBuffer();

	// shader
	ID3D11VertexShader* p_vertexShader = nullptr;
	ID3D11PixelShader* p_pixelShader = nullptr;

	// input layout
	ID3D11InputLayout* p_inputLayout = nullptr;

	// texture
	ID3D11ShaderResourceView* p_texture_SRV = nullptr;
	ID3D11ShaderResourceView* p_normalTexture_SRV = nullptr;
	ID3D11SamplerState* p_texture_SS = nullptr;

	// constant buffer
	ID3D11Buffer* p_cbMatrix;
	struct cbMatrix
	{
		XMMATRIX WVP;
		XMMATRIX World;
		XMFLOAT3 WCP;
	};

	ID3D11Buffer* p_cbLighting;
	struct cbLighting
	{
		CDirectionalLight dirLight;
		CPointLight pointLight, pointLight2, pointLight3, pointLight4;
	};

	ID3D11Buffer* p_cbParameter;
	struct cbParameter
	{
		float time;
		float roughness;
		XMFLOAT2 pad;
	};
};