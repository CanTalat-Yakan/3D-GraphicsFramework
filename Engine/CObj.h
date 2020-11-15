#pragma once
#include <vector> 
#include <d3d11.h>
#include "CVertex.h"

struct CObj 
{
	std::vector<CVertex> vertices;
	std::vector<WORD> indices;
};