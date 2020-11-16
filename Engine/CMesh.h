#pragma once
#include "CObj.h"
#include "CVertex.h"
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class CMesh
{
public:
	int init(ID3D11Device* m_pd3dDevice, CObj _obj);
	void update();
	void render(ID3D11DeviceContext* m_pd3dDevCon);
	void release();

	inline XMMATRIX getWorldMatrix() { return m_worldMatrix; }
	inline UINT getIndexCount() { return m_indexCount; }
	inline void setPosition(float x, float y, float z) { m_position = { x, y, z }; }

private:
	int initVertexBuffer(ID3D11Device* pd3dDevice, CVertex _vertices[]);
	int initIndexBuffer(ID3D11Device* pd3dDevice, WORD _indices[]);

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

