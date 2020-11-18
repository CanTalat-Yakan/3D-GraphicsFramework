#pragma once
#include "CVertex.h"
#include <vector> 
#include <d3d11.h>

struct CObj 
{
	std::vector<CVertex> vertices;
	std::vector<WORD> indices;

	UINT vertexCount = 0;
	UINT vertexStride = 0;
	UINT indexCount = 0;
};