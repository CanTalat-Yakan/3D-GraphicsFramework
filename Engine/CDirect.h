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

	ID3D11Device* getDevice() { return m_pd3dDev; }
	ID3D11DeviceContext* getDeviceContext() { return m_pd3dDevCon; }

private:
	CDirect() {}

	CWindow* m_window;

	ID3D11Device* m_pd3dDev = nullptr;						//Object for creating direct 3d objects
	ID3D11DeviceContext* m_pd3dDevCon = nullptr;			//Object for modify render pipeline
	IDXGISwapChain* m_pd3dSwapChain = nullptr;				//Holds references to front & back buffer and for switching those
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;	//Reference to back buffer
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;	//Reference to depth stencil buffer
	ID3D11RasterizerState* m_pRasterizerState = nullptr;	//Reference to rasterizer stage manipulation
	D3D11_VIEWPORT m_viewPort = {};							//Hold view port data
};

