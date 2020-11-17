#pragma once
#include <d3d9.h>
#include <DirectXMath.h>
#include "CWindow.h"

using namespace DirectX;

class CCamera
{
public:
	static CCamera& getInstance() { static CCamera instance; return instance; }
	CCamera(CCamera const&) = delete;
	void operator=(CCamera const&) = delete;

	int init();
	void update();
	void release();

	inline XMMATRIX getView() { return m_view; }
	inline XMMATRIX getProjection() { return m_projection; }
	inline XMMATRIX getViewProjectionMatrix() { return	m_view * m_projection; }
	inline XMVECTOR getCamPos() { return m_position; }
	inline float getAspectRatio() { RECT _rect = m_window->getRect(); return (float)(_rect.right - _rect.left) / (float)(_rect.bottom - _rect.top); }

private:
	CCamera() {}

	CWindow* m_window;

	void UpdateCameraVectors();

	XMVECTOR m_position = {};
	XMFLOAT2 m_mouseRot = {};

	XMVECTOR m_front = { 0, 0, 1 };
	XMVECTOR m_right = { 1, 0, 0 };
	XMVECTOR m_up = { 0, 1, 0 };

	XMMATRIX m_view = {};
	XMMATRIX m_projection = {};

};

