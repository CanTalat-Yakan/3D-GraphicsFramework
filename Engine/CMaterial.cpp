#include "CMaterial.h"
#include "CCamera.h"
#include <d3dcompiler.h>

int CMaterial::Init(LPCWSTR _shaderName)
{
#pragma region //Get Instances of DirectX and Camera
	p_d3d = &p_d3d->GetInstance();
	p_camera = &p_camera->GetInstance();
	p_lighting = &p_lighting->GetInstance();
	p_time = &p_time->GetInstance();
	p_params = &p_params->GetInstance();
#pragma endregion

#pragma region //Create SkyBox Texture
	int error = 0;
	//if (error = createTextureAndSampler(L"T_SkyBox.png", &p_skyBox_Texture_SRV) > 0) return error;
#pragma endregion

#pragma region //Create Material
	if (error = createVertexShader(_shaderName) > 0) return error;
	if (error = createPixelShader(_shaderName) > 0) return error;
	if (error = createBuffer(&p_cbMatrix, sizeof(cbMatrix)) > 0) return error;
	if (error = createBuffer(&p_cbLighting, sizeof(cbLighting)) > 0) return error;
	if (error = createBuffer(&p_cbParameter, sizeof(cbParameter)) > 0) return error;
#pragma endregion


	return 0;
}

void CMaterial::Render(XMMATRIX _worldMatrix)
{
	p_d3d->GetDeviceContext()->IASetInputLayout(p_inputLayout);
	p_d3d->GetDeviceContext()->VSSetShader(p_vertexShader, nullptr, 0);
	p_d3d->GetDeviceContext()->PSSetShader(p_pixelShader, nullptr, 0);

	setMatrixBuffer(_worldMatrix);
	SetLightingBuffer();
	SetParameterBuffer();

	p_d3d->GetDeviceContext()->PSSetShaderResources(0, 1, &p_colour_Texture_SRV);
	p_d3d->GetDeviceContext()->PSSetShaderResources(1, 1, &p_normal_Texture_SRV);
	p_d3d->GetDeviceContext()->PSSetShaderResources(2, 1, &p_height_Texture_SRV);
	p_d3d->GetDeviceContext()->PSSetShaderResources(3, 1, &p_skyBox_Texture_SRV);
	p_d3d->GetDeviceContext()->PSSetSamplers(0, 1, &p_texture_SS);
}

void CMaterial::Release()
{
	if (p_colour_Texture_SRV)
		p_colour_Texture_SRV->Release();
	p_colour_Texture_SRV = nullptr;

	if (p_normal_Texture_SRV)
		p_normal_Texture_SRV->Release();
	p_normal_Texture_SRV = nullptr;

	if (p_height_Texture_SRV)
		p_height_Texture_SRV->Release();
	p_height_Texture_SRV = nullptr;

	if (p_skyBox_Texture_SRV)
		p_skyBox_Texture_SRV->Release();
	p_skyBox_Texture_SRV = nullptr;

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
	HRESULT hr = p_d3d->GetDeviceContext()->Map(p_cbMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(hr)) return;

	cbMatrix* buffer = reinterpret_cast<cbMatrix*>(data.pData);
	buffer->WVP = _worldMatrix * p_camera->GetViewProjectionMatrix();
	buffer->World = _worldMatrix;
	buffer->WCP = p_camera->GetCamPosFloat3();

	p_d3d->GetDeviceContext()->Unmap(p_cbMatrix, 0);
	p_d3d->GetDeviceContext()->VSSetConstantBuffers(0, 1, &p_cbMatrix);
}

void CMaterial::SetLightingBuffer()
{
	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = p_d3d->GetDeviceContext()->Map(p_cbLighting, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(hr)) return;

	cbLighting* buffer = reinterpret_cast<cbLighting*>(data.pData);
	buffer->dirLight = p_lighting->DirectionalLight;
	buffer->pointLight = p_lighting->PointLight;
	buffer->pointLight2 = p_lighting->PointLight2;
	buffer->pointLight3 = p_lighting->PointLight3;
	buffer->pointLight4 = p_lighting->PointLight4;

	p_d3d->GetDeviceContext()->Unmap(p_cbLighting, 0);
	p_d3d->GetDeviceContext()->PSSetConstantBuffers(1, 1, &p_cbLighting);
}

void CMaterial::SetParameterBuffer()
{
	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = p_d3d->GetDeviceContext()->Map(p_cbParameter, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(hr)) return;

	cbParameter* buffer = reinterpret_cast<cbParameter*>(data.pData);
	buffer->time = p_time->GetTime();
	buffer->params = p_params->Parameters;

	p_d3d->GetDeviceContext()->Unmap(p_cbParameter, 0);
	p_d3d->GetDeviceContext()->PSSetConstantBuffers(2, 1, &p_cbParameter);
	p_d3d->GetDeviceContext()->VSSetConstantBuffers(3, 1, &p_cbParameter);
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

	hr = p_d3d->GetDevice()->CreateVertexShader(pVS_Buffer->GetBufferPointer(), pVS_Buffer->GetBufferSize(), NULL, &p_vertexShader);
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

	hr = p_d3d->GetDevice()->CreatePixelShader(pPS_Buffer->GetBufferPointer(), pPS_Buffer->GetBufferSize(), NULL, &p_pixelShader);
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

	HRESULT hr = p_d3d->GetDevice()->CreateInputLayout(elements, 4, _pBlob->GetBufferPointer(), _pBlob->GetBufferSize(), &p_inputLayout);
	if (FAILED(hr)) return 55;

	return 0;
}


int CMaterial::createBuffer(ID3D11Buffer** _pcb, UINT _cbSize)
{
	D3D11_BUFFER_DESC cbDESC = { 0 };
	cbDESC.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDESC.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDESC.Usage = D3D11_USAGE_DYNAMIC;
	cbDESC.ByteWidth = _cbSize;
	cbDESC.MiscFlags = 0;

	HRESULT hr = p_d3d->GetDevice()->CreateBuffer(&cbDESC, nullptr, &*_pcb);
	if (FAILED(hr)) return 50;

	return 0;
}

int CMaterial::createTextureAndSampler(LPCWSTR _textureName, ID3D11ShaderResourceView** _texture_SRV)
{
	// create texture
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(p_d3d->GetDevice(), _textureName, NULL, NULL, *&_texture_SRV, NULL);
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
	sDESC.MaxLOD = 100; //D3D11_FLOAT32_MAX;

	hr = p_d3d->GetDevice()->CreateSamplerState(&sDESC, &p_texture_SS);
	if (FAILED(hr)) return 54;

	return 0;
}
