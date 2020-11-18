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
	static CCamera& getInstance() { static CCamera instance; return instance; }
	CCamera(CCamera const&) = delete;
	void operator=(CCamera const&) = delete;

	int Init();
	void Update();
	void Release();

	inline XMMATRIX getView() { return m_view; }
	inline XMMATRIX getProjection() { return m_projection; }
	inline XMMATRIX getViewProjectionMatrix() { return	m_view * m_projection; }
	inline XMVECTOR getCamPos() { return m_position; }
	inline XMFLOAT3 getCamPosFloat3() { XMFLOAT3 r; XMStoreFloat3(&r, m_position); return r; }
	inline float getAspectRatio() { RECT _rect = m_window->getRect(); return (float)(_rect.right - _rect.left) / (float)(_rect.bottom - _rect.top); }

private:
	CCamera() {}

	void UpdateCameraVectors();

	CWindow* m_window;
	CTime* m_time;
	CInput* m_input;

	XMVECTOR m_position = { 0, 0, -1.4};
	XMFLOAT2 m_mouseRot = { 90, 0};

	XMVECTOR m_front = { 0, 0, 1 };
	XMVECTOR m_right = { 1, 0, 0 };
	XMVECTOR m_up = { 0, 1, 0 };

	XMMATRIX m_view = {};
	XMMATRIX m_projection = {};

};

