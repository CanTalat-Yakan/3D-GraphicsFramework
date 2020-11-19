#include "CDirect.h"

int CDirect::Init(bool _isWindowed)
{
#pragma region //Get Window Variables
	m_window = &m_window->GetInstance();
	HWND hWnd = m_window->getHWND();
	RECT rect = m_window->getRect();
#pragma endregion

#pragma region //Create buffer description for swapChain description
	DXGI_MODE_DESC bufferDESC = {};
	bufferDESC.Width = rect.right - rect.left;
	bufferDESC.Height = rect.bottom - rect.top;
	bufferDESC.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDESC.RefreshRate.Numerator = 60;
	bufferDESC.RefreshRate.Denominator = 1;
	bufferDESC.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	bufferDESC.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
#pragma endregion

#pragma region 	//Create device, device context & swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDESC = {};
	swapChainDESC.BufferCount = 1;
	swapChainDESC.BufferDesc = bufferDESC;
	swapChainDESC.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDESC.OutputWindow = hWnd;
	swapChainDESC.SampleDesc.Count = 1;
	swapChainDESC.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDESC.Windowed = _isWindowed;

	D3D_FEATURE_LEVEL supportedLevels[] = { D3D_FEATURE_LEVEL_10_0 };

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL, D3D_DRIVER_TYPE_HARDWARE,	//Graphic adapter /Driver type, hardware or software rendering?
		NULL, NULL,						//Reference to software module if driver type is software /Optional flags
		supportedLevels, 1,				//Supported direct 3d versions
		D3D11_SDK_VERSION,				//Api version the application was build with
		&swapChainDESC, &m_pd3dSwapChain, &m_pd3dDev,
		nullptr,						//Optional chosen feature level
		&m_pd3dDevCon);
	if (FAILED(hr)) return 11;
#pragma endregion

#pragma region 	//Create render target view, get back buffer texture before
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = m_pd3dSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr)) return 12;

	hr = m_pd3dDev->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
	if (FAILED(hr)) return 13;

	pBackBuffer->Release();
	pBackBuffer = nullptr;
#pragma endregion

#pragma region 	//Create depth stencil view
	ID3D11Texture2D* pDepthStencilBuffer = nullptr;
	D3D11_TEXTURE2D_DESC bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	bufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	bufferDesc.Width = rect.right - rect.left;
	bufferDesc.Height = rect.bottom - rect.top;
	bufferDesc.ArraySize = 1;
	bufferDesc.SampleDesc.Count = 1;

	hr = m_pd3dDev->CreateTexture2D(&bufferDesc, nullptr, &pDepthStencilBuffer);
	if (FAILED(hr)) return 14;

	hr = m_pd3dDev->CreateDepthStencilView(pDepthStencilBuffer, nullptr, &m_pDepthStencilView);
	if (FAILED(hr)) return 15;

	pDepthStencilBuffer->Release();
	pDepthStencilBuffer = nullptr;

	m_pd3dDevCon->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
#pragma endregion

#pragma region 	//Create rasterizer state
	D3D11_RASTERIZER_DESC rsDESC = {};
	rsDESC.FillMode = D3D11_FILL_SOLID;
	rsDESC.CullMode = D3D11_CULL_BACK;

	hr = m_pd3dDev->CreateRasterizerState(&rsDESC, &m_pRasterizerState);
	if (FAILED(hr)) return 16;

	m_pd3dDevCon->RSSetState(m_pRasterizerState);
#pragma endregion

#pragma region //Set viewport
	m_viewPort.Width = rect.right - rect.left;
	m_viewPort.Height = rect.bottom - rect.top;
	m_viewPort.TopLeftX = 0;
	m_viewPort.TopLeftY = 0;
	m_viewPort.MinDepth = 0;
	m_viewPort.MaxDepth = 1;

	m_pd3dDevCon->RSSetViewports(1, &m_viewPort);
#pragma endregion

	return 0;
}

void CDirect::Clear()
{
	//Clear back buffer with solid color
	const FLOAT color[] = { 1, 1, 1, 1 };
	m_pd3dDevCon->ClearRenderTargetView(m_pRenderTargetView, color);
	m_pd3dDevCon->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CDirect::Present()
{
	//Swap front and back buffer
	m_pd3dSwapChain->Present(0, 0);
}

void CDirect::Release()
{
	m_pRasterizerState->Release();
	m_pRasterizerState = nullptr;
	m_pDepthStencilView->Release();
	m_pDepthStencilView = nullptr;
	m_pRenderTargetView->Release();
	m_pRenderTargetView = nullptr;
	m_pd3dSwapChain->Release();
	m_pd3dSwapChain = nullptr;
	m_pd3dDevCon->Release();
	m_pd3dDevCon = nullptr;
	m_pd3dDev->Release();
	m_pd3dDev = nullptr;
	m_window->Release();
	m_window = nullptr;
}
