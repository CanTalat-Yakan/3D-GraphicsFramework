#pragma once
#include "CObj.h";
#include "CVertex.h";
#include <DirectXMath.h>

using namespace DirectX;

struct CTerrain
{
public:
	CObj Init(int _width, int _height, float _scale = 1.0f)
	{
		CObj obj;
		float stepSizeX = 1.0f / (_width - 1);
		float stepSizeZ = 1.0f / (_height - 1);
		std::vector<WORD> rowStarts;

		for (int x = 0; x < _width; x++)
		{
			rowStarts.push_back(obj.vertices.size());
			for (int z = 0; z < _height; z++)
			{
				obj.vertices.push_back(CVertex(
					x * stepSizeX * _scale, 0, z * stepSizeZ * _scale,
					1 - x * stepSizeZ, z * stepSizeZ,
					0, -1, 0));
			}
		}

		for (int row = 0; row < _width - 1; row++)
			for (int column = 0; column < _height - 1; column++)
			{
				obj.indices.push_back(rowStarts[row] + column);
				obj.indices.push_back(rowStarts[row] + 1 + column);
				obj.indices.push_back(rowStarts[row + 1] + column);

				obj.indices.push_back(rowStarts[row + 1] + column);
				obj.indices.push_back(rowStarts[row] + column + 1);
				obj.indices.push_back(rowStarts[row + 1] + column + 1);
			}

		return obj;
	}
};