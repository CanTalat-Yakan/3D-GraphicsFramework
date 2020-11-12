#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct CVertex
{
	CVertex() {}
	CVertex(
		float x, float y, float z,
		float u, float v,
		float nx, float ny, float nz) :
		pos(x, y, z), texCoord(u, v), normal(nx, ny, nz) {}

	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
	XMFLOAT3 normal;
};
