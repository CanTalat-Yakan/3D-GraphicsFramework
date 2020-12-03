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
	CVertex(
		float x, float y, float z,
		float u, float v,
		float nx, float ny, float nz,
		float tx, float ty, float tz) :
		pos(x, y, z), texCoord(u, v), normal(nx, ny, nz), tangent(tx, ty, tz) {}

	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
};
