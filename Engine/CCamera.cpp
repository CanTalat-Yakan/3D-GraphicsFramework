#include "CCamera.h"
#include "CTime.h"
#include "CInput.h"

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
		0.1f, 1000);	//near & far clipping plane

	return 0;
}
void CCamera::update(RECT _rect)
{
	CTime* time;
	time = &time->getInstance();
	CInput* input;
	input = &input->getInstance();

	float movementspeed = time->getDeltaTime();
	if (input->getKeyboardState(DIK_LSHIFT) & 0x80) movementspeed *= 2;
	if (input->getKeyboardState(DIK_LCONTROL) & 0x80) movementspeed *= 0.5f;
	if (input->getKeyboardState(DIK_A) & 0x80) m_position.x -= movementspeed;
	if (input->getKeyboardState(DIK_D) & 0x80) m_position.x += movementspeed;
	if (input->getKeyboardState(DIK_W) & 0x80) m_position.y += movementspeed;
	if (input->getKeyboardState(DIK_S) & 0x80) m_position.y -= movementspeed;
	if (input->getKeyboardState(DIK_Q) & 0x80) m_position.z -= movementspeed;
	if (input->getKeyboardState(DIK_E) & 0x80) m_position.z += movementspeed;
	//if (input->getKeyboardState(DIMOUSE_BUTTON0) & 0x80) m_position.z += movementspeed;
	//if (input->getMouseState().rgbButtons == DIMOUSE_BUTTON0) m_position.x -= movementspeed;

	if (m_mouseRot.y <= 1 && m_mouseRot.y >= -1)
		m_mouseRot.y += input->getMouseState().lY * 0.001f;
	if (m_mouseRot.y > 1) m_mouseRot.y = 1;
	else if (m_mouseRot.y < -1) m_mouseRot.y = -1;


	float dir = 1;
	if (m_mouseRot.x > 3 || m_mouseRot.x < -3)
		dir *= -1;

	//if (dir < 0)
		//m_mouseRot.x -= input->getMouseState().lX * 0.002f;
	//if (dir > 0)
		m_mouseRot.x += input->getMouseState().lX * 0.002f;


	m_camPos = XMVectorSet(m_position.x, m_position.z, m_position.y, 0);
	m_camTarget = XMVectorSet(m_position.x + m_mouseRot.x, m_position.z - m_mouseRot.y, m_position.y + dir, 0);
	m_camUp = XMVectorSet(0, 1, 0, 0);

	m_view = XMMatrixLookAtLH(m_camPos, m_camTarget, m_camUp);


	m_projection = XMMatrixPerspectiveFovLH(0.5f * 3.141f, (float)(_rect.right - _rect.left) / (float)(_rect.bottom - _rect.top), 0.1f, 1000);
}

void CCamera::release()
{
}
