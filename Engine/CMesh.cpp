#include "CMesh.h"
#include "CVertex.h"
#include "CTime.h"

int CMesh::init(ID3D11Device* pD3DDevice)
{
	int error = 0;

	if (error = initVertexBuffer(pD3DDevice) > 0) return error;
	if (error = initIndexBuffer(pD3DDevice) > 0) return error;

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

int CMesh::initVertexBuffer(ID3D11Device* pD3DDevice)
{
	m_vertexCount = 4 * 6;
	m_vertexStride = sizeof(CVertex);

	float hs = 0.5f;

	// quad - with uv & normals
	CVertex vertices[] =
	{
		// Front Face
		CVertex(-hs, -hs, -hs, 0, 1, 0, 0, 1), //Bottom		Left
		CVertex(-hs,  hs, -hs, 0, 0, 0, 0, 1), //Top		Left
		CVertex( hs,  hs, -hs, 1, 0, 0, 0, 1), //Top		Right
		CVertex( hs, -hs, -hs, 1, 1, 0, 0, 1), //Bottom		Right

		// Left Face
		CVertex(-hs, -hs,  hs, 0, 1, 1, 0, 0), //Bottom		Left
		CVertex(-hs,  hs,  hs, 0, 0, 1, 0, 0), //Top		Left
		CVertex(-hs,  hs, -hs, 1, 0, 1, 0, 0), //Top		Right
		CVertex(-hs, -hs, -hs, 1, 1, 1, 0, 0), //Bottom		Right

		// Back Face
		CVertex( hs, -hs,  hs, 0, 1, 0, 0, -1), //Bottom	Left
		CVertex( hs,  hs,  hs, 0, 0, 0, 0, -1), //Top		Left
		CVertex(-hs,  hs,  hs, 1, 0, 0, 0, -1), //Top		Right
		CVertex(-hs, -hs,  hs, 1, 1, 0, 0, -1), //Bottom	Right

		// Right Face
		CVertex( hs, -hs, -hs, 0, 1, -1, 0, 0), //Bottom	Left
		CVertex( hs,  hs, -hs, 0, 0, -1, 0, 0), //Top		Left
		CVertex( hs,  hs,  hs, 1, 0, -1, 0, 0), //Top		Right
		CVertex( hs, -hs,  hs, 1, 1, -1, 0, 0), //Bottom	Right

		// Top Face
		CVertex(-hs,  hs, -hs, 0, 1, 0, -1, 0), //Bottom	Left
		CVertex( hs,  hs, -hs, 0, 0, 0, -1, 0), //Top		Left
		CVertex( hs,  hs,  hs, 1, 0, 0, -1, 0), //Top		Right
		CVertex(-hs,  hs,  hs, 1, 1, 0, -1, 0), //Bottom	Right
									 	
		// Bottom Face					 	
		CVertex( hs, -hs,  hs, 0, 1, 0, 1, 0), //Bottom	Left
		CVertex(-hs, -hs,  hs, 0, 0, 0, 1, 0), //Top		Left
		CVertex(-hs, -hs, -hs, 1, 0, 0, 1, 0), //Top		Right
		CVertex( hs, -hs, -hs, 1, 1, 0, 1, 0), //Bottom	Right
	};

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // buffer type
	desc.ByteWidth = m_vertexCount * m_vertexStride; // buffer size

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = vertices;

	HRESULT hr = pD3DDevice->CreateBuffer(&desc, &data, &m_pVertexBuffer);
	if (FAILED(hr)) return 30;

	return 0;
}

int CMesh::initIndexBuffer(ID3D11Device* pD3DDevice)
{
	m_indexCount = 6 * 6;

	// quad
	WORD indices[] = {
		// Front Face
		0, 1, 2,
		0, 2, 3,
		// Left Face
		4, 5, 6,
		4, 6, 7,
		// Back Face
		8, 9, 10,
		8, 10, 11,
		// Right Face
		12, 13, 14,
		12, 14, 15,
		// Top Face
		16, 17, 18,
		16, 18, 19,
		// Bottom Face
		20, 21, 22,
		20, 22, 23,
	};

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER; // buffer type
	desc.ByteWidth = m_indexCount * sizeof(WORD); // buffer size

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = indices;

	HRESULT hr = pD3DDevice->CreateBuffer(&desc, &data, &m_pIndexBuffer);
	if (FAILED(hr)) return 30;


	return 0;
}
