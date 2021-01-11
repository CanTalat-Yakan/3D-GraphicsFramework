#pragma once
#include "CWindow.h"
//#include <d3d9.h>
#include <d3d11.h>
//#include <D3DX10.h>
//#include <D3DX11.h>
//#include <xnamath.h>
//#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "d3dx11.lib")
//#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "dxguid.lib")

class CDirect
{
public:
	static CDirect& GetInstance() { static CDirect instance; return instance; }
	CDirect(CDirect const&) = delete;
	void operator=(CDirect const&) = delete;

	int Init(bool _isWindowed);
	void Release();
	void Clear();
	void Present();

	ID3D11Device* GetDevice() { return p_d3dDev; }
	ID3D11DeviceContext* GetDeviceContext() { return p_d3dDevCon; }
	void GetRender(ID3D11Buffer* _vertexBuffer, UINT _vertexStride, ID3D11Buffer* _indexBuffer, UINT _indexCount);

private:
	CDirect() {}

	CWindow* p_window;

	ID3D11Device* p_d3dDev = nullptr;						//Object for creating direct 3d objects
	ID3D11DeviceContext* p_d3dDevCon = nullptr;				//Object for modify render pipeline
	IDXGISwapChain* p_d3dSwapChain = nullptr;				//Holds references to front & back buffer and for switching those
	ID3D11RenderTargetView* p_renderTargetView = nullptr;	//Reference to back buffer
	ID3D11DepthStencilView* p_depthStencilView = nullptr;	//Reference to depth stencil buffer
	ID3D11RasterizerState* p_rasterizerState = nullptr;		//Reference to rasterizer stage manipulation
	ID3D11BlendState* p_blendState = nullptr;				//Reference to blend stage
	D3D11_VIEWPORT m_viewPort = {};							//Hold view port data
};

