#include "CCamera.h"
#include "CTime.h"
#include "CInput.h"

int CCamera::init()
{
	m_window = &m_window->getInstance();

	// ---CAMERA
	XMVECTOR m_camPos = XMVectorSet(0, 0, -1.5f, 0);
	XMVECTOR m_camTarget = XMVectorSet(0, 0, 1, 0);
	XMVECTOR m_camUp = XMVectorSet(0, 1, 0, 0);

	m_view = XMMatrixLookAtLH(m_camPos, m_camTarget, m_camUp);
	m_projection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(80), //FOV in rad (optional: 0.5f * 3.141f)
		getAspectRatio(),	//Aspect ratio
		0.1f, 1000);			//near & far clipping plane

	m_position = m_camPos;

	return 0;
}

void CCamera::update()
{
	CTime* time;
	time = &time->getInstance();
	CInput* input;
	input = &input->getInstance();

	float movementspeed = time->getDeltaTime() * 2;
	if (input->getKeyboardState(DIK_LSHIFT) & 0x80) movementspeed *= 2;
	if (input->getKeyboardState(DIK_LCONTROL) & 0x80) movementspeed *= 0.5f;
	if (input->getKeyboardState(DIK_A) & 0x80) m_position += m_right * movementspeed;
	if (input->getKeyboardState(DIK_D) & 0x80) m_position -= m_right * movementspeed;
	if (input->getKeyboardState(DIK_W) & 0x80) m_position += m_front * movementspeed;
	if (input->getKeyboardState(DIK_S) & 0x80) m_position -= m_front * movementspeed;
	if (input->getKeyboardState(DIK_E) & 0x80) m_position += m_up * movementspeed;
	if (input->getKeyboardState(DIK_Q) & 0x80) m_position -= m_up * movementspeed;

	if (input->getMouseState().rgbButtons[0] != 0)
	{
		input->hideMouse();
		m_mouseRot.x -= input->getMouseState().lX * 0.2f;
		m_mouseRot.y -= input->getMouseState().lY * 0.2f;
	}

	m_mouseRot.y =
		(m_mouseRot.y > 80) ? 80 :
		(m_mouseRot.y < -80) ? -80 :
		m_mouseRot.y;

	UpdateCameraVectors();

	m_projection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(80),
		getAspectRatio(),
		0.1f, 1000);
}

void CCamera::release()
{
}

void CCamera::UpdateCameraVectors()
{
	XMFLOAT3 front = {
		front.x = cos(XMConvertToRadians(m_mouseRot.x)) * cos(XMConvertToRadians(m_mouseRot.y)),
		front.y = sin(XMConvertToRadians(m_mouseRot.y)),
		front.z = sin(XMConvertToRadians(m_mouseRot.x)) * cos(XMConvertToRadians(m_mouseRot.y)) };

	m_front = XMVector3Normalize(XMVectorSet(front.x, front.y, front.z, 0));
	m_right = XMVector3Normalize(XMVector3Cross(m_front, m_up));

	m_view = XMMatrixLookAtLH(m_position, m_position + m_front, m_up);
}
