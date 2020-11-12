#pragma once
#include <d3d9.h>
#include <DirectXMath.h>

using namespace DirectX;

class CCamera
{
public:
	int init(RECT _rect);
	void update();
	void release();

	inline XMMATRIX getView() { return m_view; }
	inline XMMATRIX getProjection() { return m_projection; }
	inline XMMATRIX getViewProjectionMatrix() { return	m_view * m_projection; }

private:
	XMFLOAT3 m_position = {};


	XMVECTOR m_camPos = {};
	XMVECTOR m_camTarget = {};
	XMVECTOR m_camUp = {};
	XMMATRIX m_view = {};
	XMMATRIX m_projection = {};

};

