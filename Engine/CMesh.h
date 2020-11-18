#pragma once
#include "CObj.h"
#include "CVertex.h"
#include "CDirect.h"
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class CMesh
{
public:
	int init(CObj _obj);
	void Update();
	void render();
	void release();

	inline XMMATRIX getWorldMatrix() { return m_worldMatrix; }
	inline UINT getIndexCount() { return m_indexCount; }
	inline void setPosition(float x, float y, float z) { m_position = { x, y, z }; }

private:
	CDirect* m_d3d;

	int initVertexBuffer(CVertex _vertices[]);
	int initIndexBuffer(WORD _indices[]);

	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* m_pIndexBuffer = nullptr;

	UINT m_vertexCount = 0;
	UINT m_vertexStride = 0;
	UINT m_indexCount = 0;

	XMMATRIX m_worldMatrix = {};
	XMFLOAT3 m_position = {};
	XMFLOAT4 m_rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMFLOAT3 m_scale = { 1.0f, 1.0f, 1.0f };
};

