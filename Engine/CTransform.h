#pragma once
#include "DirectXMath.h"

using namespace DirectX;

struct CTransform
{
	XMMATRIX m_worldMatrix = {};
	XMFLOAT3 m_position = { 0, 0, 0 };
	XMFLOAT4 m_rotation = { 0, 0, 0, 1 };
	XMFLOAT3 m_scale = { 1, 1, 1 };

	inline XMMATRIX GetWorldMatrix() { return m_worldMatrix; }
	inline XMVECTOR GetRotation() { return XMLoadFloat4(&m_rotation); }
	inline XMFLOAT4 GetRotationFloat4() { return m_rotation; }
	inline XMFLOAT3 GetRotationFloat3() { XMFLOAT3 r; XMStoreFloat3(&r, GetRotation()); return r; }

	inline void SetPosition(float _x, float _y, float _z) { m_position = { _x, _y, _z }; }
	inline void SetPosition(XMFLOAT3 _pos) { m_position = { _pos.x, _pos.y, _pos.z }; }
	inline void SetScale(float _x, float _y, float _z) { m_scale = { _x, _y, _z }; }
	inline void SetScale(XMFLOAT3 _scale) { m_scale = _scale; }
	inline void SetScale(float _x) { SetScale(_x, _x, _x); }
	inline void SetRotation(float _x, float _y, float _z) { SetRotationDeg(XMConvertToRadians(_x), XMConvertToRadians(_y), XMConvertToRadians(_z)); }
	inline void SetRotationAdditive(float _x, float _y, float _z) { SetRotation(XMConvertToRadians(_x + m_rotation.x), XMConvertToRadians(_y + m_rotation.y), XMConvertToRadians(_z + m_rotation.z)); }
	inline void SetRotationDeg(float _x, float _y, float _z) { SetRotation(XMQuaternionRotationRollPitchYaw(_x, _y, _z)); }
	inline void SetRotationDegAdditive(float _x, float _y, float _z) { SetRotation(XMQuaternionMultiply(GetRotation(), XMQuaternionRotationRollPitchYaw(_x, _y, _z))); }
	inline void SetRotation(float _x) { SetRotation(_x, _x, _x); }
	inline void SetRotationDeg(float _x) { SetRotationDeg(_x, _x, _x); }
	inline void SetRotation(XMFLOAT4 _rot) { m_rotation = _rot; }
	inline void SetRotation(XMVECTOR _quaternion) { XMStoreFloat4(&m_rotation, _quaternion); }

	inline void Update()
	{
		XMVECTOR quaternion = XMLoadFloat4(&m_rotation);

		XMMATRIX translation = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
		XMMATRIX rotation = XMMatrixRotationQuaternion(quaternion);
		XMMATRIX scale = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);

		m_worldMatrix = scale * rotation * translation;
	}
};
