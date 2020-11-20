#pragma once
#include "IMono.h"

class GScene : public MonoBehaviour
{
public:
	// Inherited via MonoBehaviour
	virtual void Init() override;
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Release() override;

private:
	CMesh m_skyBox = {};
	CMesh m_plane = {};
	CMesh m_cube = {};
	CMesh m_cylinder = {};
	CMesh m_sphere = {};
	CMesh m_sphere2 = {};
	CMesh m_sphere3 = {};
	CMesh m_bird = {};
	CMaterial m_material_Sky = {};
	CMaterial m_material_Proto = {};
	CMaterial m_material_Cells = {};
	CMaterial m_material_CellShader = {};
	CMaterial m_material_Fresnel = {};
	CMaterial m_material_Bird = {};
	CLight m_light = {};
	CObjLoader m_obj = {};

};

