#pragma once
#include <d3d9.h>
#include <DirectXMath.h>

using namespace DirectX;

class CCamera
{
public:
	static CCamera& getInstance() { static CCamera instance; return instance; }
	CCamera(CCamera const&) = delete;
	void operator=(CCamera const&) = delete;

	int init(RECT _rect);
	void update(RECT _rect);
	void release();

	inline XMMATRIX getView() { return m_view; }
	inline XMMATRIX getProjection() { return m_projection; }
	inline XMMATRIX getViewProjectionMatrix() { return	m_view * m_projection; }
	inline XMVECTOR getCamPos() { return m_camPos; }

private:
	CCamera() {}

	XMFLOAT3 m_position = {};

	XMVECTOR m_camPos = {};
	XMVECTOR m_camTarget = {};
	XMVECTOR m_camUp = {};
	XMMATRIX m_view = {};
	XMMATRIX m_projection = {};
	XMFLOAT2 m_mouseRot = {};

};

