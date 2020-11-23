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
	void Update();
	void Render();
	void Update_Render(CMaterial _material);
	void Release();

	inline XMMATRIX GetWorldMatrix() { return m_worldMatrix; }
	inline XMMATRIX GetTranslationMatrix() { return m_translationMatrix; }
	inline UINT GetVertexCount() { return m_vertexCount; }
	inline UINT GetVertexStride() { return m_vertexStride; }
	inline UINT GetIndexCount() { return m_indexCount; }
	inline UINT GetIndexStride() { return sizeof(WORD); }

	inline void SetPosition(float _x, float _y, float _z) { m_position = { _x, _y, _z }; }
	inline void SetPosition(XMFLOAT3 _pos) { m_position = { _pos.x, _pos.y, _pos.z }; }
	inline void SetScale(float _x, float _y, float _z) { m_scale = { _x, _y, _z }; }
	inline void SetScale(XMFLOAT3 _scale) { m_scale = _scale; }
	inline void SetScale(float _x) { SetScale(_x, _x, _x); }
	inline void SetRotation(float _x, float _y, float _z) { SetRotationDeg(XMConvertToRadians(_x), XMConvertToRadians(_y), XMConvertToRadians(_z)); }
	inline void SetRotationDeg(float _x, float _y, float _z) { SetRotation(XMQuaternionMultiply(GetRotation(), XMQuaternionRotationRollPitchYaw(_x, _y, _z))); }
	inline void SetRotation(float _x) { SetRotation(_x, _x, _x); }
	inline void SetRotationDeg(float _x) { SetRotationDeg(_x, _x, _x); }
	inline void SetRotation(XMFLOAT4 _rot) { m_rotation = _rot; }
	inline void SetRotation(XMVECTOR _quaternion) { XMStoreFloat4(&m_rotation, _quaternion); }
	inline XMVECTOR GetRotation() { return XMLoadFloat4(&m_rotation); }
	inline XMFLOAT4 GetRotationFloat4() { return m_rotation; }

private:
	CDirect* p_d3d;
	CTime* p_time;

	int initVertexBuffer(CVertex _vertices[]);
	int initIndexBuffer(WORD _indices[]);

	ID3D11Buffer* p_vertexBuffer = nullptr;
	ID3D11Buffer* p_indexBuffer = nullptr;

	std::vector<CVertex> m_vertices = {};
	std::vector<WORD> m_indices = {};
	UINT m_vertexCount = 0;
	UINT m_vertexStride = 0;
	UINT m_indexCount = 0; 

	XMMATRIX m_worldMatrix = {};
	XMMATRIX m_translationMatrix = {};
	XMFLOAT3 m_position = { 0, 0, 0 };
	XMFLOAT4 m_rotation = { 0, 0, 0, 1 };
	XMFLOAT3 m_scale = { 1, 1, 1 };
};

