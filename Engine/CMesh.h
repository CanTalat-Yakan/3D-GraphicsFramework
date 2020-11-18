#pragma once
#include "CObj.h"
#include "CVertex.h"
#include "CDirect.h"
#include "CTime.h"
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class CMesh
{
public:
	int Init(CObj _obj);
	void Update();
	void Render();
	void Release();

	inline XMMATRIX getWorldMatrix() { return m_worldMatrix; }
	inline UINT getVertexCount() { return m_vertexCount; }
	inline UINT getVertexStride() { return m_vertexStride; }
	inline UINT getIndexCount() { return m_indexCount; }
	inline void setPosition(float x, float y, float z) { m_position = { x, y, z }; }
	inline void setPosition(XMFLOAT3 _pos) { m_position = { _pos.x, _pos.y, _pos.z }; }
	inline void setScale(float x, float y, float z) { m_scale = { x, y, z }; }
	inline void setScale(XMFLOAT3 _scale) { m_scale = _scale; }
	inline void setRotation(float x, float y, float z) { m_rotation = { x, y, z, 1 }; }
	inline void setRotation(XMFLOAT4 _rot) { m_rotation = _rot; }
	inline void setRotation(XMVECTOR _quaternion) { XMStoreFloat4(&m_rotation, _quaternion); }
	inline XMVECTOR getRotation() { return XMLoadFloat4(&m_rotation); }
	inline XMFLOAT4 getRotationFloat4() { return m_rotation; }

private:
	CDirect* m_d3d;
	CTime* m_time;

	int initVertexBuffer(CVertex _vertices[]);
	int initIndexBuffer(WORD _indices[]);

	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* m_pIndexBuffer = nullptr;

	UINT m_vertexCount = 0;
	UINT m_vertexStride = 0;
	UINT m_indexCount = 0;

	XMMATRIX m_worldMatrix = {};
	XMFLOAT3 m_position = { 0, 0, 0 };
	XMFLOAT4 m_rotation = { 0, 0, 0, 1 };
	XMFLOAT3 m_scale = { 1, 1, 1 };
};

