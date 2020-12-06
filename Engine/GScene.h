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
	GObject m_cube = {};
	GObject m_sphere, m_sphere2, m_sphere3 = {};
	GObject m_cylinder, m_cylinder2, m_cylinder3 = {};
	GObject m_duck, m_duck2, m_duck3 = {};

	CMaterial m_mat_Sky = {};
	CMaterial m_mat_Standard, m_mat_Standard2, m_mat_Standard3 = {};
	CMaterial m_mat_Toon = {};
	CMaterial m_mat_Fresnel = {};
	CMaterial m_mat_duck = {};
};

