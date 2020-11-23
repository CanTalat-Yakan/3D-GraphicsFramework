#pragma once
#include <d3d9.h>
#include <DirectXMath.h>
#include "CTime.h"
#include "CInput.h"
#include "CWindow.h"

using namespace DirectX;

class CCamera
{
public:
	static CCamera& GetInstance() { static CCamera instance; return instance; }
	CCamera(CCamera const&) = delete;
	void operator=(CCamera const&) = delete;

	int Init();
	void Update();
	void Release();

	inline XMMATRIX GetView() { return m_view; }
	inline XMMATRIX GetProjection() { return m_projection; }
	inline XMMATRIX GetViewProjectionMatrix() { return	m_view * m_projection; }
	inline XMVECTOR GetCamPos() { return m_position; }
	inline XMFLOAT3 GetCamPosFloat3() { XMFLOAT3 r; XMStoreFloat3(&r, m_position); return r; }
	inline float GetAspectRatio() { RECT _rect = p_window->getRect(); return (float)(_rect.right - _rect.left) / (float)(_rect.bottom - _rect.top); }
	inline void SetPosition(float _x, float _y, float _z) { m_position = XMVectorSet(_x, _y, _z, 1); }
	inline void SetRotation(float _x, float _y) { m_mouseRot = XMFLOAT2(_x, _y); }

private:
	CCamera() {}

	void updateCameraVectors();

	CWindow* p_window;
	CTime* p_time;
	CInput* m_input;

	XMVECTOR m_position = { 0, 0, 0};
	XMFLOAT2 m_mouseRot = { 0, 0};

	XMVECTOR m_front = { 0, 0, 1 };
	XMVECTOR m_right = { 1, 0, 0 };
	XMVECTOR m_up = { 0, 1, 0 };

	XMMATRIX m_view = {};
	XMMATRIX m_projection = {};

};

