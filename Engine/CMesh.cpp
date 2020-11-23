#include "CMesh.h"
#include "CVertex.h"

int CMesh::Init(CObj _obj)
{
#pragma region //Get Instance of DirectX and Time
	p_d3d = &p_d3d->GetInstance(); 
	p_time = &p_time->GetInstance();
#pragma endregion

#pragma region //Set Variables
	m_vertexCount = _obj.vertexCount;
	m_vertexStride = _obj.vertexStride;
	m_indexCount = _obj.indexCount;
#pragma endregion

#pragma region //Create Buffer
	int error = 0;
	if (error = initVertexBuffer(&_obj.vertices[0], GetVertexCount() * GetVertexStride()) > 0) return error;
	if (error = initIndexBuffer(&_obj.indices[0], GetIndexCount() * GetIndexStride()) > 0) return error;
#pragma endregion

	return 0;
}

void CMesh::Update()
{
	XMVECTOR quaternion = XMLoadFloat4(&m_rotation);

	XMMATRIX translation = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	XMMATRIX rotation = XMMatrixRotationQuaternion(quaternion);
	XMMATRIX scale = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);

	m_worldMatrix = scale * rotation * translation;
	m_translationMatrix = translation;
}

void CMesh::Render()
{
	static UINT offset = 0;

	p_d3d->getDeviceContext()->IASetVertexBuffers(0, 1, &p_vertexBuffer, &m_vertexStride, &offset);
	p_d3d->getDeviceContext()->IASetIndexBuffer(p_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	p_d3d->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	p_d3d->getDeviceContext()->DrawIndexed(GetIndexCount(), 0, 0);
}

void CMesh::Update_Render(CMaterial _material)
{
	_material.Render(GetWorldMatrix(), GetTranslationMatrix());
	Update();
	Render();
}

void CMesh::Release()
{
	if (p_vertexBuffer != NULL)
		p_vertexBuffer->Release();
	p_vertexBuffer = nullptr;

	if (p_indexBuffer != NULL)
		p_indexBuffer->Release();
	p_indexBuffer = nullptr;
}


int CMesh::initVertexBuffer(CVertex _vertices[], UINT _byteWidth)
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // buffer type
	desc.ByteWidth = _byteWidth; // buffer size

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = _vertices;

	HRESULT hr = p_d3d->getDevice()->CreateBuffer(&desc, &data, &p_vertexBuffer);
	if (FAILED(hr)) return 30;

	return 0;
}

int CMesh::initIndexBuffer(WORD _indices[], UINT _byteWidth)
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER; // buffer type
	desc.ByteWidth = _byteWidth; // buffer size

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = _indices;

	HRESULT hr = p_d3d->getDevice()->CreateBuffer(&desc, &data, &p_indexBuffer);
	if (FAILED(hr)) return 30;

	return 0;
}
