#include "CCamera.h"
#include "CTime.h"

int CCamera::init(RECT _rect)
{
	// ---CAMERA

	m_camPos = XMVectorSet(0, 0, -1.5f, 0);
	m_camTarget = XMVectorSet(0, 0, 1, 0);
	m_camUp = XMVectorSet(0, 1, 0, 0);

	m_view = XMMatrixLookAtLH(m_camPos, m_camTarget, m_camUp);
	m_projection = XMMatrixPerspectiveFovLH(
		0.5f * 3.141f,	//FOV in rad
		(float)(_rect.right - _rect.left) / (float)(_rect.bottom - _rect.top), //Aspect ratio
		0.1f, 1000);		//near & far clipping plane

	return 0;
}

void CCamera::update()
{
	CTime* time;
	time = &time->getInstance();
	float movementspeed = time->getDeltaTime();

	if (GetAsyncKeyState(VK_LSHIFT) & 0x8000) movementspeed *= 2;
	if (GetAsyncKeyState(VK_LCONTROL) & 0x8000) movementspeed *= 0.5f;
	if (GetAsyncKeyState('A') & 0x8000) m_position.x -= movementspeed;
	if (GetAsyncKeyState('D') & 0x8000) m_position.x += movementspeed;
	if (GetAsyncKeyState('W') & 0x8000) m_position.y += movementspeed;
	if (GetAsyncKeyState('S') & 0x8000) m_position.y -= movementspeed;
	if (GetAsyncKeyState('Q') & 0x8000) m_position.z -= movementspeed;
	if (GetAsyncKeyState('E') & 0x8000) m_position.z += movementspeed;

	m_camPos = XMVectorSet(m_position.x, m_position.z, m_position.y, 0);
	m_camTarget = XMVectorSet(m_position.x, m_position.z, 1, 0);
	m_camUp = XMVectorSet(0, 1, 0, 0);

	m_view = XMMatrixLookAtLH(m_camPos, m_camTarget, m_camUp);
}

void CCamera::release()
{
}
