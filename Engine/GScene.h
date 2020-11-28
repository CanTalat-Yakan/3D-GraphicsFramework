#pragma once
#include "IMono.h"

class GScene : public MonoBehaviour
{
public:
	// Inherited via MonoBehaviour
	virtual void Init() override;
	virtual void Awake() override;
	virtual void Start() override;
	virtual void EarlyUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Release() override;

private:
	GObject m_skyBox = {};
	GObject m_plane = {};
	GObject m_cube, m_cube2, m_cube3 = {};
	GObject m_sphere, m_sphere2, m_sphere3 = {};
	GObject m_cylinder, m_cylinder2, m_cylinder3 = {};
	GObject m_bird, m_bird2, m_bird3 = {};

	CMaterial m_mat_Sky = {};
	CMaterial m_mat_Standard, m_mat_Standard2, m_mat_Standard3 = {};
	CMaterial m_mat_Toon = {};
	CMaterial m_mat_Fresnel = {};
	CMaterial m_mat_Bird = {};

	CLight m_light = {};
	CTransform m_lightTransform;
};

