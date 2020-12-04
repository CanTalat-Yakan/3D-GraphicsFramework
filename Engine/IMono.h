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

	CDirect* D3d = {};
	CCamera* Camera = {};
	CTime* Time = {};

	inline void GetInstances() 
	{
		D3d = &D3d->GetInstance();
		Camera = &Camera->GetInstance();
		Time = &Time->GetInstance();
	}
};