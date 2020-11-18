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
	void Render(XMMATRIX _worldMatrix);
	void Release();

	void SetLightBuffer(const CLight& _light);

private:
	CDirect* m_d3d;
	CCamera* m_camera;


	int createVertexShader(LPCWSTR _shaderName);
	int createPixelShader(LPCWSTR _shaderName);
	int createInputLayout(ID3DBlob* _pBlob);
	int createMatrixBuffer();
	int createPixelShaderBuffer();
	int createTextureAndSampler(LPCWSTR _textureName);

	void setMatrixBuffer(XMMATRIX _worldMatrix, XMMATRIX _viewProjectionMatrix);

	// shader
	ID3D11VertexShader* m_pvertexShader = nullptr;
	ID3D11PixelShader* m_ppixelShader = nullptr;

	// input layout
	ID3D11InputLayout* m_pinputLayout = nullptr;

	// texture
	ID3D11ShaderResourceView* m_ptexture_SRV = nullptr;
	ID3D11SamplerState* m_ptexture_SS = nullptr;

	//constant buffer
	ID3D11Buffer* m_pcbPerObj = nullptr;
	struct cbPerObject
	{
		XMMATRIX WVP;
		XMMATRIX World;
		XMVECTOR WCP;
	};

	ID3D11Buffer* m_pcbPerFrame = nullptr;
	struct cbPerFrame
	{
		CLight Light;
	};
};