#include "CMaterial.h"
#include "CCamera.h"
#include <d3dcompiler.h>

int CMaterial::Init(LPCWSTR _shaderName, LPCWSTR _textureName)
{
#pragma region //Get Instances of DirectX and Camera
	p_d3d = &p_d3d->GetInstance();
	p_camera = &p_camera->GetInstance();
#pragma endregion

#pragma region //Create Material
	int error = 0;
	if (error = createVertexShader(_shaderName) > 0) return error;
	if (error = createPixelShader(_shaderName) > 0) return error;
	if (error = createMatrixBuffer() > 0) return error;
	if (error = createPixelShaderBuffer() > 0) return error;
	if (error = createTextureAndSampler(_textureName) > 0) return error;
#pragma endregion

	return 0;
}

void CMaterial::Render(XMMATRIX _worldMatrix, XMMATRIX _translationMatrix)
{
	p_d3d->getDeviceContext()->IASetInputLayout(p_inputLayout);
	p_d3d->getDeviceContext()->VSSetShader(p_vertexShader, nullptr, 0);
	p_d3d->getDeviceContext()->PSSetShader(p_pixelShader, nullptr, 0);

	setMatrixBuffer(_worldMatrix, _translationMatrix);

	p_d3d->getDeviceContext()->PSSetShaderResources(0, 1, &p_texture_SRV);
	p_d3d->getDeviceContext()->PSSetSamplers(0, 1, &p_texture_SS);
}

void CMaterial::Release()
{
	if (p_texture_SRV)
		p_texture_SRV->Release();
	p_texture_SRV = nullptr;
	if (p_texture_SS)
		p_texture_SS->Release();
	p_vertexShader->Release();
	p_vertexShader = nullptr;
	p_pixelShader->Release();
	p_pixelShader = nullptr;
	p_inputLayout->Release();
	p_inputLayout = nullptr;
	p_texture_SS = nullptr;
	p_cbPerObj->Release();
	p_cbPerObj = nullptr;
	p_cbPerFrame->Release();
	p_cbPerFrame = nullptr;
	p_d3d = nullptr;
	p_camera = nullptr;
}


void CMaterial::SetLightBuffer(const CLight& _light)
{
	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = p_d3d->getDeviceContext()->Map(p_cbPerFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(hr)) return;

	cbPerFrame* buffer = reinterpret_cast<cbPerFrame*>(data.pData);
	buffer->Light = _light;

	p_d3d->getDeviceContext()->Unmap(p_cbPerFrame, 0);

	p_d3d->getDeviceContext()->PSSetConstantBuffers(0, 1, &p_cbPerFrame);
}

void CMaterial::setMatrixBuffer(XMMATRIX _worldMatrix, XMMATRIX _translationMatrix)
{
	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = p_d3d->getDeviceContext()->Map(p_cbPerObj, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(hr)) return;
	cbPerObject* buffer = reinterpret_cast<cbPerObject*>(data.pData);

	buffer->WVP = _worldMatrix * p_camera->GetViewProjectionMatrix();
	buffer->World = _worldMatrix;
	buffer->Transl = _translationMatrix;
	buffer->WCP = p_camera->GetCamPosFloat3();

	p_d3d->getDeviceContext()->Unmap(p_cbPerObj, 0);
	p_d3d->getDeviceContext()->VSSetConstantBuffers(0, 1, &p_cbPerObj);
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

	hr = p_d3d->getDevice()->CreateVertexShader(pVS_Buffer->GetBufferPointer(), pVS_Buffer->GetBufferSize(), NULL, &p_vertexShader);
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

	hr = p_d3d->getDevice()->CreatePixelShader(pPS_Buffer->GetBufferPointer(), pPS_Buffer->GetBufferSize(), NULL, &p_pixelShader);
	if (FAILED(hr)) return 59;

	pPS_Buffer->Release();
	pPS_Buffer = nullptr;

	return 0;
}

int CMaterial::createInputLayout(ID3DBlob* _pBlob)
{
	D3D11_INPUT_ELEMENT_DESC elements[3] = {};

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

	HRESULT hr = p_d3d->getDevice()->CreateInputLayout(elements, 3, _pBlob->GetBufferPointer(), _pBlob->GetBufferSize(), &p_inputLayout);
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

	HRESULT hr = p_d3d->getDevice()->CreateBuffer(&cbDESC, nullptr, &p_cbPerObj);
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

	HRESULT hr = p_d3d->getDevice()->CreateBuffer(&cbDESC, nullptr, &p_cbPerFrame);
	if (FAILED(hr)) return 51;

	return 0;
}

int CMaterial::createTextureAndSampler(LPCWSTR _textureName)
{
	// create texture
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(p_d3d->getDevice(), _textureName, NULL, NULL, &p_texture_SRV, NULL);
	if (FAILED(hr)) return 56;

	// create sampler state
	D3D11_SAMPLER_DESC sDESC = {};
	sDESC.Filter = D3D11_FILTER_ANISOTROPIC;
	sDESC.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sDESC.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sDESC.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sDESC.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sDESC.MaxAnisotropy = 16;
	sDESC.MinLOD = 0;
	sDESC.MaxLOD = D3D11_FLOAT32_MAX;

	hr = p_d3d->getDevice()->CreateSamplerState(&sDESC, &p_texture_SS);
	if (FAILED(hr)) return 54;

	return 0;
}
