#pragma once
#include "CTransform.h";
#include "CMesh.h";
#include "CMaterial.h"

struct GObject 
{
	CTransform transform;
	CMesh mesh;
	CMaterial material;

	inline void Release() { mesh.Release(); }

	inline void Update_Render()
	{
		transform.Update();
		material.Render(transform.GetWorldMatrix());
		mesh.Render();
	}
};
