#include "CMaterial.h"
#include "CCamera.h"
#include <d3dcompiler.h>

int CMaterial::init(ID3D11Device* _pd3dDev, LPCWSTR _textureName, LPCWSTR _shaderName)
{
	int error = 0;

	m_pd3dDev = _pd3dDev;

	if (error = createVertexShader(_shaderName) > 0) return error;
	if (error = createPixelShader(_shaderName) > 0) return error;
	if (error = createMatrixBuffer() > 0) return error;
	if (error = createPixelShaderBuffer() > 0) return error;
	if (error = createTextureAndSampler(_textureName) > 0) return error;

	return 0;
}

void CMaterial::render(ID3D11DeviceContext* _pd3dDevCon, XMMATRIX _worldMatrix, XMMATRIX _viewProjectionMatrix)
{
	_pd3dDevCon->IASetInputLayout(m_pinputLayout);
	_pd3dDevCon->VSSetShader(m_pvertexShader, nullptr, 0);
	_pd3dDevCon->PSSetShader(m_ppixelShader, nullptr, 0);

	setMatrixBuffer(_pd3dDevCon, _worldMatrix, _viewProjectionMatrix);

	_pd3dDevCon->PSSetShaderResources(0, 1, &m_ptexture_SRV);
	_pd3dDevCon->PSSetSamplers(0, 1, &m_ptexture_SS);
}

void CMaterial::release()
{
	m_pvertexShader->Release();
	m_pvertexShader = nullptr;
	m_ppixelShader->Release();
	m_ppixelShader = nullptr;
	m_pinputLayout->Release();
	m_pinputLayout = nullptr;
	m_ptexture_SRV->Release();
	m_ptexture_SRV = nullptr;
	m_ptexture_SS->Release();
	m_ptexture_SS = nullptr;
	m_pcbPerObj->Release();
	m_pcbPerObj = nullptr;
	m_pcbPerFrame->Release();
	m_pcbPerFrame = nullptr;
}


void CMaterial::setLightBuffer(ID3D11DeviceContext* _pd3dDevCon, const CLight& _light)
{
	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = _pd3dDevCon->Map(m_pcbPerFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(hr)) return;

	cbPerFrame* buffer = reinterpret_cast<cbPerFrame*>(data.pData);
	buffer->Light = _light;

	_pd3dDevCon->Unmap(m_pcbPerFrame, 0);

	_pd3dDevCon->PSSetConstantBuffers(0, 1, &m_pcbPerFrame);
}

void CMaterial::setMatrixBuffer(ID3D11DeviceContext* _pd3dDevCon, XMMATRIX _worldMatrix, XMMATRIX _viewProjectionMatrix)
{
	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = _pd3dDevCon->Map(m_pcbPerObj, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(hr)) return;
	cbPerObject* buffer = reinterpret_cast<cbPerObject*>(data.pData);

	XMMATRIX wvp = XMMatrixTranspose(_worldMatrix * _viewProjectionMatrix);
	_worldMatrix = XMMatrixTranspose(_worldMatrix);

	buffer->WVP = wvp;
	buffer->World = _worldMatrix;
	CCamera* camera;
	camera = &camera->getInstance();
	buffer->WCP = camera->getCamPos();

	_pd3dDevCon->Unmap(m_pcbPerObj, 0);
	_pd3dDevCon->VSSetConstantBuffers(0, 1, &m_pcbPerObj);
}


int CMaterial::createVertexShader(LPCWSTR _shaderName)
{
	ID3DBlob* pVS_Buffer = {};
	HRESULT hr = D3DX11CompileFromFile(
		_shaderName,		// shader filename
		0, 0,				// optional macros & includes
		"VS",				// entry point function
		"vs_4_0",			// shader type & version
		0, 0, 0,			// optional flags
		&pVS_Buffer,		// compiled blob target
		0, 0);				// optional blob for all compile errors
	
	if (FAILED(hr)) return 50;

	hr = m_pd3dDev->CreateVertexShader(pVS_Buffer->GetBufferPointer(), pVS_Buffer->GetBufferSize(), NULL, &m_pvertexShader);
	if (FAILED(hr)) return 53;

	int error = createInputLayout(pVS_Buffer);
	if (error > 0) return error;

	pVS_Buffer->Release();
	pVS_Buffer = nullptr;

	return 0;
}

int CMaterial::createPixelShader(LPCWSTR _shaderName)
{
	ID3DBlob* pPS_Buffer = {};
	HRESULT hr = D3DX11CompileFromFile(_shaderName, 0, 0, "PS", "ps_4_0", 0, 0, 0, &pPS_Buffer, 0, 0);
	if (FAILED(hr)) return 57;

	hr = m_pd3dDev->CreatePixelShader(pPS_Buffer->GetBufferPointer(), pPS_Buffer->GetBufferSize(), NULL, &m_ppixelShader);
	if (FAILED(hr)) return 59;

	pPS_Buffer->Release();
	pPS_Buffer = nullptr;

	return 0;
}

int CMaterial::createInputLayout(ID3DBlob* _pBlob)
{
	D3D11_INPUT_ELEMENT_DESC elements[3] = {};
	/*{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL",0,  DXGI_FORMAT_R32G32B32_FLOAT,0,20,D3D11_INPUT_PER_VERTEX_DATA,0},
	};*/

	// position
	elements[0].SemanticName = "POSITION"; // element type
	elements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT; // element format/size
	elements[0].InputSlot = D3D11_INPUT_PER_VERTEX_DATA;

	// uv
	elements[1].SemanticName = "TEXCOORD";
	elements[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	elements[1].InputSlot = D3D11_INPUT_PER_VERTEX_DATA;
	elements[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	// normal
	elements[2].SemanticName = "NORMAL";
	elements[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	elements[2].InputSlot = D3D11_INPUT_PER_VERTEX_DATA;
	elements[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	
	HRESULT hr = m_pd3dDev->CreateInputLayout(elements, 3, _pBlob->GetBufferPointer(), _pBlob->GetBufferSize(), &m_pinputLayout);
	if (FAILED(hr)) return 55;

	return 0;
}


int CMaterial::createMatrixBuffer()
{
	D3D11_BUFFER_DESC cbDESC = {};
	cbDESC.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDESC.ByteWidth = sizeof(cbPerObject);
	cbDESC.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDESC.MiscFlags = 0;
	cbDESC.Usage = D3D11_USAGE_DYNAMIC;

	HRESULT hr = m_pd3dDev->CreateBuffer(&cbDESC, nullptr, &m_pcbPerObj);
	if (FAILED(hr)) return 58;

	return 0;
}

int CMaterial::createPixelShaderBuffer()
{
	D3D11_BUFFER_DESC cbDESC = {};
	cbDESC.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDESC.ByteWidth = sizeof(cbPerFrame);
	cbDESC.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDESC.MiscFlags = 0;
	cbDESC.Usage = D3D11_USAGE_DYNAMIC;

	HRESULT hr = m_pd3dDev->CreateBuffer(&cbDESC, nullptr, &m_pcbPerFrame);
	if (FAILED(hr)) return 51;

	return 0;
}

int CMaterial::createTextureAndSampler(LPCWSTR _textureName)
{
	// create texture
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(m_pd3dDev, _textureName, NULL, NULL, &m_ptexture_SRV, NULL);
	if (FAILED(hr)) return 56;

	// create sampler state
	D3D11_SAMPLER_DESC sDESC = {};
	sDESC.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sDESC.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sDESC.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sDESC.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sDESC.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sDESC.MinLOD = 0;
	sDESC.MaxLOD = D3D11_FLOAT32_MAX;

	hr = m_pd3dDev->CreateSamplerState(&sDESC, &m_ptexture_SS);
	if (FAILED(hr)) return 54;

	return 0;
}
