#pragma once

#include <d3d11.h>
#include <D3DX11.h>
#include <D3DX10.h>
#include <xnamath.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

struct cbPerObject
{
	XMMATRIX WVP;
};
D3D11_INPUT_ELEMENT_DESC layout[] =
{
	{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
	{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0},
};
class CDRX
{
public:
	UINT* numElements;
	ID3D11Device* d3dev;
	ID3D11DeviceContext* d3devcon;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthStencilBuffer;

	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;
	ID3D11VertexShader* VS;
	ID3D10Blob* VS_Buffer;
	ID3D11PixelShader* PS;
	ID3D10Blob* PS_Buffer;
	ID3D11InputLayout* VertexLayout;

	ID3D11Buffer* cbPerObjectBuffer;
	XMMATRIX WVP, World, View, Projection;
	XMMATRIX Translate, Scale, RotY;
	XMVECTOR camPos, camTarget, camUp;

	cbPerObject cbpO;
	float Width, Height;

	CDRX(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed, HWND _hWnd);
	static bool Update(float deltaTime, CDRX _rndr);
	static bool Draw(float deltaTime, CDRX _rndr);
};