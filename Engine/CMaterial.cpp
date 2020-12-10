#include "CMaterial.h"
#include "CCamera.h"
#include <d3dcompiler.h>

int CMaterial::Init(LPCWSTR _shaderName, LPCWSTR _textureName)
{
#pragma region //Get Instances of DirectX and Camera
	p_d3d = &p_d3d->GetInstance();
	p_camera = &p_camera->GetInstance();
	p_lighting = &p_lighting->GetInstance();
	p_time = &p_time->GetInstance();
#pragma endregion

#pragma region //Create Material
	int error = 0;
	if (error = createVertexShader(_shaderName) > 0) return error;
	if (error = createPixelShader(_shaderName) > 0) return error;
	if (error = createBuffer(&p_cbMatrix, sizeof(cbMatrix)) > 0) return error;
	if (error = createBuffer(&p_cbLighting, sizeof(cbLighting)) > 0) return error;
	if (error = createBuffer(&p_cbParameter, sizeof(cbParameter)) > 0) return error;
	if (error = createTextureAndSampler(_textureName, &p_texture_SRV) > 0) return error;
#pragma endregion

	return 0;
}

int CMaterial::Init(LPCWSTR _shaderName, LPCWSTR _textureName, LPCWSTR _normalTextureName)
{
#pragma region Use previous complete init method
	Init(_shaderName, _textureName);
#pragma endregion

#pragma region Add additional Textures
	int error = 0;
	if (error = createTextureAndSampler(_normalTextureName, &p_normalTexture_SRV) > 0) return error;
#pragma endregion

	return 0;
}

void CMaterial::Render(XMMATRIX _worldMatrix)
{
	p_d3d->getDeviceContext()->IASetInputLayout(p_inputLayout);
	p_d3d->getDeviceContext()->VSSetShader(p_vertexShader, nullptr, 0);
	p_d3d->getDeviceContext()->PSSetShader(p_pixelShader, nullptr, 0);

	setMatrixBuffer(_worldMatrix);
	SetLightingBuffer();
	SetParameterBuffer();

	p_d3d->getDeviceContext()->PSSetShaderResources(0, 1, &p_texture_SRV);
	p_d3d->getDeviceContext()->PSSetShaderResources(1, 1, &p_normalTexture_SRV);
	p_d3d->getDeviceContext()->PSSetSamplers(0, 1, &p_texture_SS);
}

void CMaterial::Release()
{
	if (p_texture_SRV)
		p_texture_SRV->Release();
	p_texture_SRV = nullptr;

	if (p_normalTexture_SRV)
		p_normalTexture_SRV->Release();
	p_normalTexture_SRV = nullptr;

	if (p_texture_SS)
		p_texture_SS->Release();
	p_texture_SS = nullptr;

	p_vertexShader->Release();
	p_vertexShader = nullptr;
	p_pixelShader->Release();
	p_pixelShader = nullptr;
	p_inputLayout->Release();
	p_inputLayout = nullptr;
	p_cbMatrix->Release();
	p_cbMatrix = nullptr;
	p_cbLighting->Release();
	p_cbLighting = nullptr;
	p_cbParameter->Release();
	p_cbParameter = nullptr;
	p_d3d = nullptr;
	p_camera = nullptr;
}


void CMaterial::setMatrixBuffer(XMMATRIX _worldMatrix)
{
	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = p_d3d->getDeviceContext()->Map(p_cbMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(hr)) return;

	cbMatrix* buffer = reinterpret_cast<cbMatrix*>(data.pData);
	buffer->WVP = _worldMatrix * p_camera->GetViewProjectionMatrix();
	buffer->World = _worldMatrix;
	buffer->WCP = p_camera->GetCamPosFloat3();

	p_d3d->getDeviceContext()->Unmap(p_cbMatrix, 0);
	p_d3d->getDeviceContext()->VSSetConstantBuffers(0, 1, &p_cbMatrix);
}

void CMaterial::SetLightingBuffer()
{
	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = p_d3d->getDeviceContext()->Map(p_cbLighting, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(hr)) return;

	cbLighting* buffer = reinterpret_cast<cbLighting*>(data.pData);
	buffer->dirLight = p_lighting->DirectionalLight;
	buffer->pointLight = p_lighting->PointLight;

	p_d3d->getDeviceContext()->Unmap(p_cbLighting, 0);
	p_d3d->getDeviceContext()->PSSetConstantBuffers(1, 1, &p_cbLighting);
}

void CMaterial::SetParameterBuffer()
{
	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = p_d3d->getDeviceContext()->Map(p_cbParameter, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(hr)) return;

	cbParameter* buffer = reinterpret_cast<cbParameter*>(data.pData);
	buffer->time = (float)p_time->getTime();
	buffer->deltaTime = (float)p_time->getDeltaTime();

	p_d3d->getDeviceContext()->Unmap(p_cbParameter, 0);
	p_d3d->getDeviceContext()->PSSetConstantBuffers(2, 1, &p_cbParameter);
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
	D3D11_INPUT_ELEMENT_DESC elements[4] = {};

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
	// tangent
	elements[3].SemanticName = "TANGENT";
	elements[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	elements[3].InputSlot = D3D11_INPUT_PER_VERTEX_DATA;
	elements[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	
	HRESULT hr = p_d3d->getDevice()->CreateInputLayout(elements, 4, _pBlob->GetBufferPointer(), _pBlob->GetBufferSize(), &p_inputLayout);
	if (FAILED(hr)) return 55;

	return 0;
}


int CMaterial::createBuffer(ID3D11Buffer** _pcb, UINT _cbSize)
{
	D3D11_BUFFER_DESC cbDESC = {};
	cbDESC.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDESC.ByteWidth = _cbSize;
	cbDESC.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDESC.MiscFlags = 0;
	cbDESC.Usage = D3D11_USAGE_DYNAMIC;

	HRESULT hr = p_d3d->getDevice()->CreateBuffer(&cbDESC, nullptr, &*_pcb);
	if (FAILED(hr)) return 50;

	return 0;
}

int CMaterial::createTextureAndSampler(LPCWSTR _textureName, ID3D11ShaderResourceView** _texture_SRV)
{
	// create texture
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(p_d3d->getDevice(), _textureName, NULL, NULL, *&_texture_SRV, NULL);
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
