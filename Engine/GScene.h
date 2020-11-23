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
	CMesh m_skyBox = {};
	CMesh m_plane = {};
	CMesh m_cube = {};
	CMesh m_cube2 = {};
	CMesh m_cube3 = {};
	CMesh m_sphere = {};
	CMesh m_sphere2 = {};
	CMesh m_sphere3 = {};
	CMesh m_cylinder = {};
	CMesh m_cylinder2 = {};
	CMesh m_cylinder3 = {};
	CMesh m_bird = {};

	CMaterial m_material_Sky = {};
	CMaterial m_material_Standard = {};
	CMaterial m_material_Standard2 = {};
	CMaterial m_material_Standard3 = {};
	CMaterial m_material_Toon = {};
	CMaterial m_material_Fresnel = {};
	CMaterial m_material_Bird = {};

	CLight m_light = {};
	CObjLoader m_obj = {};

};

