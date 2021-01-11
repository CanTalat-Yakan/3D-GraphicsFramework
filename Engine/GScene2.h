#pragma once
#include "IMono.h"

class GScene2 : public MonoBehaviour
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
	GObject m_sphere = {};

	CMaterial m_mat_Sky = {};
	CMaterial m_mat_Reflection = {};
};

