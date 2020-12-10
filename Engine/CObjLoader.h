#pragma once
#include "CObj.h"
#include "CTerrain.h"
#include <string>
#include <d3d11.h>

enum class EPrimitives
{
	Cube,
	Plane,
	Cylinder,
	Pyramid,
	Sphere,
};
class CObjLoader
{
public:
	CObj Load(LPCWSTR _fileName);
	CObj Load(EPrimitives _primitiveType);
	CObj LoadTerrain(int _width, int _height, int _scale);
	void SafeFile(std::string _fileName, CObj _obj);

private:
	void loadPrimVertices();
	void loadPrimIndices();
	void loadFile(LPCWSTR _fileName);
	void readFile(std::ifstream* _fileStream);
	std::vector<std::string> splitString(std::string _s, std::string _delim);

	CObj m_obj = {};
	EPrimitives m_prim = {};
};