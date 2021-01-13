#pragma once
#include "IMono.h"

class CScene : public MonoBehaviour
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
	GObject m_sphere, m_sphere2, m_sphere3, m_sphere4, m_sphere5, m_sphere6 = {};
	GObject m_cylinder, m_cylinder2, m_cylinder3 = {};
	GObject m_duck, m_duck2, m_duck3 = {};
	GObject m_volcano = {};
	GObject m_water = {};
	GObject m_terrain = {};
	GObject m_pointLightVisualizer = {};

	CMaterial m_mat_Sky = {};
	CMaterial m_mat_Plane, m_mat_Grid, m_mat_Normal = {};
	CMaterial m_mat_Toon = {};
	CMaterial m_mat_Bubble = {};
	CMaterial m_mat_Duck = {};
	CMaterial m_mat_Volcano = {};
	CMaterial m_mat_Water = {};
	CMaterial m_mat_Terrain = {};
	CMaterial m_mat_Unlit = {};
};

