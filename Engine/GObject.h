#pragma once
#include "CTransform.h";
#include "CMesh.h";

struct GObject 
{
	CTransform transform;
	CMesh mesh;

	inline void Release() { mesh.Release(); }

	inline void Update_Render(CMaterial _material)
	{
		transform.Update();
		_material.Render(transform.GetWorldMatrix(), transform.GetTranslationMatrix());
		mesh.Render();
	}
};
