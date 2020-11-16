#include "CMesh.h"
#include "CVertex.h"
#include "CTime.h"

int CMesh::init(ID3D11Device* pD3DDevice, CObj _obj)
{
	int error = 0;

	if (error = initVertexBuffer(pD3DDevice, &_obj.vertices[0]) > 0) return error;
	if (error = initIndexBuffer(pD3DDevice, &_obj.indices[0]) > 0) return error;

	return 0;
}

void CMesh::update()
{
	CTime* time;
	time = &time->getInstance();

	XMVECTOR quaternion = XMLoadFloat4(&m_rotation);
	float rotationspeed = time->getDeltaTime() * 0.1f;
	quaternion = XMQuaternionMultiply(quaternion, XMQuaternionRotationRollPitchYaw(rotationspeed, rotationspeed, rotationspeed));
	XMStoreFloat4(&m_rotation, quaternion);

	XMMATRIX translation = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	XMMATRIX rotation = XMMatrixRotationQuaternion(quaternion);
	XMMATRIX scale = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);

	m_worldMatrix = scale * rotation * translation;
}

void CMesh::render(ID3D11DeviceContext* pD3DDeviceContext)
{
	static UINT offset = 0;

	pD3DDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_vertexStride, &offset);
	pD3DDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	pD3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void CMesh::release()
{
	if (m_pVertexBuffer != NULL)
		m_pVertexBuffer->Release();
	m_pVertexBuffer = nullptr;

	if (m_pIndexBuffer != NULL)
		m_pIndexBuffer->Release();
	m_pIndexBuffer = nullptr;
}

int CMesh::initVertexBuffer(ID3D11Device* pD3DDevice, CVertex _vertices[])
{
	m_vertexCount = 4 * 6;
	m_vertexStride = sizeof(CVertex);


	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // buffer type
	desc.ByteWidth = m_vertexCount * m_vertexStride; // buffer size

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = _vertices;

	HRESULT hr = pD3DDevice->CreateBuffer(&desc, &data, &m_pVertexBuffer);
	if (FAILED(hr)) return 30;

	return 0;
}

int CMesh::initIndexBuffer(ID3D11Device* pD3DDevice, WORD _indices[])
{
	m_indexCount = 6 * 6;

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER; // buffer type
	desc.ByteWidth = m_indexCount * sizeof(WORD); // buffer size

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = _indices;

	HRESULT hr = pD3DDevice->CreateBuffer(&desc, &data, &m_pIndexBuffer);
	if (FAILED(hr)) return 30;


	return 0;
}
