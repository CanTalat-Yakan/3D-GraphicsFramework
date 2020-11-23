#pragma once
#include "CLight.h"
#include "CDirect.h"
#include "CCamera.h"
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
	void Render(XMMATRIX _worldMatrix, XMMATRIX _translationMatrix);
	void Release();

	void SetLightBuffer(const CLight& _light);

private:
	CDirect* p_d3d;
	CCamera* p_camera;


	int createVertexShader(LPCWSTR _shaderName);
	int createPixelShader(LPCWSTR _shaderName);
	int createInputLayout(ID3DBlob* _pBlob);
	int createMatrixBuffer();
	int createPixelShaderBuffer();
	int createTextureAndSampler(LPCWSTR _textureName);

	void setMatrixBuffer(XMMATRIX _worldMatrix, XMMATRIX _translationMatrix);

	// shader
	ID3D11VertexShader* p_vertexShader = nullptr;
	ID3D11PixelShader* p_pixelShader = nullptr;

	// input layout
	ID3D11InputLayout* p_inputLayout = nullptr;

	// texture
	ID3D11ShaderResourceView* p_texture_SRV = nullptr;
	ID3D11SamplerState* p_texture_SS = nullptr;

	//constant buffer
	ID3D11Buffer* p_cbPerObj = nullptr;
	struct cbPerObject
	{
		XMMATRIX WVP;
		XMMATRIX World;
		XMMATRIX Transl;
		XMFLOAT3 WCP;
	};

	ID3D11Buffer* p_cbPerFrame = nullptr;
	struct cbPerFrame
	{
		CLight Light;
	};
};