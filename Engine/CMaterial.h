#pragma once
#include <d3d11.h>
#include <D3DX11.h>
#include <DirectXMath.h>
#include "CLight.h"

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3dx11.lib")

using namespace DirectX;

class CMaterial
{
public:
	int init(ID3D11Device* _pd3dDev, LPCWSTR _textureName);
	void render(ID3D11DeviceContext* _pd3dDevCon, XMMATRIX _worldMatrix, XMMATRIX _viewProjectionMatrix);
	void release();

	void setLight(ID3D11DeviceContext* _pd3dDevCon, const CLight& _light);

private:
	int createVertexShader(ID3D11Device* _pd3dDev);
	int createPixelShader(ID3D11Device* _pd3dDev);
	int createInputLayout(ID3D11Device* _pd3dDev, ID3DBlob* _pBlob);
	int createMatrixBuffer(ID3D11Device* _pd3dDev);
	int createPixelShaderBuffer(ID3D11Device* _pd3dDev);
	int createTextureAndSampler(ID3D11Device* _pd3dDev, LPCWSTR _textureName);

	void setMatrixBuffer(ID3D11DeviceContext* _pd3dDevCon, XMMATRIX _worldMatrix, XMMATRIX _viewProjectionMatrix);

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
		XMFLOAT4X4 WVP;
		XMFLOAT4X4 World;
	};

	ID3D11Buffer* m_pcbPerFrame = nullptr;
	struct cbPerFrame
	{
		CLight Light;
	};
};