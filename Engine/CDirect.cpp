#include "CDirect.h"

int CDirect::Init(bool _isWindowed)
{
#pragma region //Get Window Variables
	p_window = &p_window->GetInstance();
	HWND hWnd = p_window->getHWND();
	RECT rect = p_window->getRect();
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
		&swapChainDESC, &p_d3dSwapChain, &p_d3dDev,
		nullptr,						//Optional chosen feature level
		&p_d3dDevCon);
	if (FAILED(hr)) return 11;
#pragma endregion

#pragma region 	//Create render target view, get back buffer texture before
	ID3D11Texture2D* pbackBuffer = nullptr;
	hr = p_d3dSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pbackBuffer));
	if (FAILED(hr)) return 12;

	hr = p_d3dDev->CreateRenderTargetView(pbackBuffer, nullptr, &p_renderTargetView);
	if (FAILED(hr)) return 13;

	pbackBuffer->Release();
	pbackBuffer = nullptr;
#pragma endregion

#pragma region 	//Create depth stencil view
	ID3D11Texture2D* pdepthStencilBuffer = nullptr;
	D3D11_TEXTURE2D_DESC bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	bufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	bufferDesc.Width = rect.right - rect.left;
	bufferDesc.Height = rect.bottom - rect.top;
	bufferDesc.ArraySize = 1;
	bufferDesc.SampleDesc.Count = 1;

	hr = p_d3dDev->CreateTexture2D(&bufferDesc, nullptr, &pdepthStencilBuffer);
	if (FAILED(hr)) return 14;

	hr = p_d3dDev->CreateDepthStencilView(pdepthStencilBuffer, nullptr, &p_depthStencilView);
	if (FAILED(hr)) return 15;

	pdepthStencilBuffer->Release();
	pdepthStencilBuffer = nullptr;

	p_d3dDevCon->OMSetRenderTargets(1, &p_renderTargetView, p_depthStencilView);
#pragma endregion

#pragma region 	//Create rasterizer state
	D3D11_RASTERIZER_DESC rasterizerDESC = {};
	rasterizerDESC.FillMode = D3D11_FILL_SOLID;
	rasterizerDESC.CullMode = D3D11_CULL_BACK;

	hr = p_d3dDev->CreateRasterizerState(&rasterizerDESC, &p_rasterizerState);
	if (FAILED(hr)) return 16;

	p_d3dDevCon->RSSetState(p_rasterizerState);



	//Create Blend State
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));

	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	rtbd.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
	rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

	blendDesc.RenderTarget[0] = rtbd;

	hr = p_d3dDev->CreateBlendState(&blendDesc, &p_blendState);
#pragma endregion

#pragma region //Set viewport
	m_viewPort.Width = rect.right - rect.left;
	m_viewPort.Height = rect.bottom - rect.top;
	m_viewPort.TopLeftX = 0;
	m_viewPort.TopLeftY = 0;
	m_viewPort.MinDepth = 0;
	m_viewPort.MaxDepth = 1;

	p_d3dDevCon->RSSetViewports(1, &m_viewPort);
#pragma endregion

	return 0;
}

void CDirect::Clear()
{
	//Clear back buffer with solid color
	const FLOAT color[] = { 1, 1, 1, 1 };
	p_d3dDevCon->ClearRenderTargetView(p_renderTargetView, color);
	p_d3dDevCon->ClearDepthStencilView(p_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CDirect::Present()
{
	//Swap front and back buffer
	p_d3dSwapChain->Present(0, 0);
}

void CDirect::Release()
{
	p_rasterizerState->Release();
	p_rasterizerState = nullptr;
	p_depthStencilView->Release();
	p_depthStencilView = nullptr;
	p_renderTargetView->Release();
	p_renderTargetView = nullptr;
	p_d3dSwapChain->Release();
	p_d3dSwapChain = nullptr;
	p_d3dDevCon->Release();
	p_d3dDevCon = nullptr;
	p_d3dDev->Release();
	p_d3dDev = nullptr;
	p_window->Release();
	p_window = nullptr;
}
