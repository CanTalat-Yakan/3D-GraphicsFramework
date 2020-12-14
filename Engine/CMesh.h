#pragma once
#include "CObj.h"
#include "CVertex.h"
#include "CDirect.h"
#include "CMaterial.h"
#include "CTime.h"
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class CMesh
{
public:
	int Init(CObj _obj);
	void Render();
	void Release();

	inline UINT GetVertexCount() { return m_vertexCount; }
	inline UINT GetVertexStride() { return m_vertexStride; }
	inline UINT GetIndexCount() { return m_indexCount; }
	inline UINT GetIndexStride() { return m_indexStride; }

private:
	CDirect* p_d3d;
	CTime* p_time;

	int initVertexBuffer(CVertex _vertices[], UINT _byteWidth);
	int initIndexBuffer(WORD _indices[], UINT _byteWidth);

	ID3D11Buffer* p_vertexBuffer = nullptr;
	ID3D11Buffer* p_indexBuffer = nullptr;

	UINT m_vertexCount = 0;
	UINT m_vertexStride = 0;
	UINT m_indexCount = 0; 
	UINT m_indexStride = 0;
	bool m_triangles = false;
};

