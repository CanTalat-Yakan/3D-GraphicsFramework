#include "CMesh.h"
#include "CVertex.h"

int CMesh::Init(CObj _obj)
{
#pragma region //Get Instance of DirectX and Time
	m_d3d = &m_d3d->GetInstance(); 
	m_time = &m_time->GetInstance();
#pragma endregion

#pragma region //Set Variables
	m_vertices = _obj.vertices;
	m_indices = _obj.indices;
	m_vertexCount = _obj.vertexCount;
	m_vertexStride = _obj.vertexStride;
	m_indexCount = _obj.indexCount;
#pragma endregion

#pragma region //Create Buffer
	int error = 0;
	if (error = initVertexBuffer(&m_vertices[0]) > 0) return error;
	if (error = initIndexBuffer(&m_indices[0]) > 0) return error;
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

	m_d3d->getDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_vertexStride, &offset);
	m_d3d->getDeviceContext()->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	m_d3d->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_d3d->getDeviceContext()->DrawIndexed(GetIndexCount(), 0, 0);
}

void CMesh::Update_Render(CMaterial _material)
{
	_material.Render(GetWorldMatrix(), GetTranslationMatrix());
	Update();
	Render();
}

void CMesh::Release()
{
	if (m_pVertexBuffer != NULL)
		m_pVertexBuffer->Release();
	m_pVertexBuffer = nullptr;

	if (m_pIndexBuffer != NULL)
		m_pIndexBuffer->Release();
	m_pIndexBuffer = nullptr;
}


int CMesh::initVertexBuffer(CVertex _vertices[])
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // buffer type
	desc.ByteWidth = GetVertexCount() * GetVertexStride(); // buffer size

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = _vertices;

	HRESULT hr = m_d3d->getDevice()->CreateBuffer(&desc, &data, &m_pVertexBuffer);
	if (FAILED(hr)) return 30;

	return 0;
}

int CMesh::initIndexBuffer(WORD _indices[])
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER; // buffer type
	desc.ByteWidth = GetIndexCount() * GetIndexStride(); // buffer size

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = _indices;

	HRESULT hr = m_d3d->getDevice()->CreateBuffer(&desc, &data, &m_pIndexBuffer);
	if (FAILED(hr)) return 30;

	return 0;
}
