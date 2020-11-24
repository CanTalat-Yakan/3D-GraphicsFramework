#pragma once
#include "CMesh.h";
#include "CMaterial.h";
#include "CObjLoader.h"
#include "CLight.h";
#include "CCamera.h";
#include "CTime.h";
#include "GObject.h";

class MonoBehaviour
{
public:
	virtual void Init() = 0;
	virtual void Awake() = 0;
	virtual void Start() = 0;
	virtual void EarlyUpdate() = 0;
	virtual void Update() = 0;
	virtual void LateUpdate() = 0;
	virtual void Release() = 0;

	CDirect* d3d = {};
	CCamera* camera = {};
	CTime* time = {};

	inline void InitCore() 
	{
		d3d = &d3d->GetInstance();
		camera = &camera->GetInstance();
		time = &time->GetInstance();
	}
};