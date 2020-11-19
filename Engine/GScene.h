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
	CMesh m_cube2 = {};
	CMaterial m_material00 = {};
	CMaterial m_material01 = {};
	CMaterial m_material02 = {};
	CLight m_light = {};
	CObjLoader m_obj = {};

};

