#include "CObjLoader.h"
#include <fstream>
#include <vector>

CObj CObjLoader::load(LPCWSTR _fileName)
{
	loadFile(_fileName);

	return m_obj;
}

CObj CObjLoader::load(EPrimitives _primitiveType)
{
	m_prim = _primitiveType;

	loadPrimVertices();
	loadPrimIndices();

	return m_obj;
}

void CObjLoader::loadPrimVertices()
{
	switch (m_prim)
	{
	case EPrimitives::Cube:
	{
		float hs = 0.5f;
		// quad - with uv & normals
		m_obj.vertices = {
			// Front Face
			CVertex(-hs, -hs, -hs, 0, 1, 0, 0, 1),	//Bottom	Left
			CVertex(-hs, hs, -hs, 0, 0, 0, 0, 1),	//Top		Left
			CVertex(hs, hs, -hs, 1, 0, 0, 0, 1),	//Top		Right
			CVertex(hs, -hs, -hs, 1, 1, 0, 0, 1),	//Bottom	Right

			// Left Face
			CVertex(-hs, -hs, hs, 0, 1, 1, 0, 0),	//Bottom	Left
			CVertex(-hs, hs, hs, 0, 0, 1, 0, 0),	//Top		Left
			CVertex(-hs, hs, -hs, 1, 0, 1, 0, 0),	//Top		Right
			CVertex(-hs, -hs, -hs, 1, 1, 1, 0, 0),	//Bottom	Right

			// Back Face
			CVertex(hs, -hs, hs, 0, 1, 0, 0, -1),	//Bottom	Left
			CVertex(hs, hs, hs, 0, 0, 0, 0, -1),	//Top		Left
			CVertex(-hs, hs, hs, 1, 0, 0, 0, -1),	//Top		Right
			CVertex(-hs, -hs, hs, 1, 1, 0, 0, -1),	//Bottom	Right

			// Right Face
			CVertex(hs, -hs, -hs, 0, 1, -1, 0, 0),	//Bottom	Left
			CVertex(hs, hs, -hs, 0, 0, -1, 0, 0),	//Top		Left
			CVertex(hs, hs, hs, 1, 0, -1, 0, 0),	//Top		Right
			CVertex(hs, -hs, hs, 1, 1, -1, 0, 0),	//Bottom	Right

			// Top Face
			CVertex(-hs, hs, -hs, 1, 1, 0, -1, 0),	//Top		Right
			CVertex(-hs, hs, hs, 0, 1, 0, -1, 0),	//Bottom	Right
			CVertex(hs, hs, hs, 0, 0, 0, -1, 0),	//Bottom	Left
			CVertex(hs, hs, -hs, 1, 0, 0, -1, 0),	//Top		Left

			// Base Face					 	
			CVertex(-hs, -hs, hs, 0, 0, 0, 1, 0),	//Bottom	Left
			CVertex(-hs, -hs, -hs, 1, 0, 0, 1, 0),	//Top		Left
			CVertex(hs, -hs, -hs, 1, 1, 0, 1, 0),	//Top		Right
			CVertex(hs, -hs, hs, 0, 1, 0, 1, 0),	//Bottom	Right
		};
		m_obj.vertexCount = 4 * 6;
		m_obj.vertexStride = sizeof(CVertex);
		break;
	}
	case EPrimitives::Plane:
	{
		float hs = 5;
		// quad - with uv & normals
		m_obj.vertices = {
			// Face
			CVertex(-hs, hs, 0, 0, 1, 0, -1, 0),	//Top		Right
			CVertex(-hs, hs, 0, 0, 0, 0, -1, 0),	//Bottom	Right
			CVertex(hs, hs, 0, 1, 0, 0, -1, 0),		//Bottom	Left
			CVertex(hs, hs, 0, 1, 1, 0, -1, 0),		//Top		Left
		};
		m_obj.vertexCount = 1 * 4;
		m_obj.vertexStride = sizeof(CVertex);
		break;
	}
	case EPrimitives::Cylinder:
	{

		break;
	}
	case EPrimitives::Pyramid:
	{

		break;
	}
	default:
		break;
	}
}

void CObjLoader::loadPrimIndices()
{
	switch (m_prim)
	{
	case EPrimitives::Cube:
	{
		// quad
		m_obj.indices = {
			// Front Face
			0, 1, 2,
			0, 2, 3,
			// Left Face
			4, 5, 6,
			4, 6, 7,
			// Back Face
			8, 9, 10,
			8, 10, 11,
			// Right Face
			12, 13, 14,
			12, 14, 15,
			// Top Face
			16, 17, 18,
			16, 18, 19,
			// Bottom Face
			20, 21, 22,
			20, 22, 23,
		};
		m_obj.indexCount = 6 * 6;
		break;
	}
	case EPrimitives::Plane:
	{
		// quad
		m_obj.indices = {
			// Face
			0, 1, 2,
			0, 2, 3,
		};
		m_obj.indexCount = 1 * 6;
		break;
	}
	case EPrimitives::Cylinder:
	{

		break;
	}
	case EPrimitives::Pyramid:
	{

		break;
	}
	default:
		break;
	}
}

void CObjLoader::loadFile(LPCWSTR _fileName)
{
	std::ifstream fileStream(_fileName);
	if (!fileStream.is_open())
		return;

	readFile(&fileStream);

	fileStream.close();
}

void CObjLoader::readFile(std::ifstream* _fileStream)
{
	std::vector<XMFLOAT3> positions = {};
	std::vector<XMFLOAT2> uv = {};
	std::vector<XMFLOAT3> normals = {};
	std::vector<XMFLOAT3> faceInfo = {};
	std::vector<CVertex> vertices = {};
	std::vector<std::string> split_f = {};
	std::vector<std::string> split_l = {};
	std::string line;
	int faceCount = 0;
	int vertexCount = 0;
	int indexCount = 0;

	while (std::getline(*_fileStream, line))
	{
		split_l = splitString(line, " ");
		//Vertex
		if (split_l[0] == "v")
			positions.push_back(
				XMFLOAT3(std::stof(split_l[1]), std::stof(split_l[2]), std::stof(split_l[3])));
		//UV
		if (split_l[0] == "vt")
			uv.push_back(
				XMFLOAT2(std::stof(split_l[1]), std::stof(split_l[2])));
		//Normal
		if (split_l[0] == "vn")
			normals.push_back(
				XMFLOAT3(std::stof(split_l[1]), std::stof(split_l[2]), std::stof(split_l[3])));
		//Face
		if (split_l[0] == "f")
		{
			faceCount++;
			//Vertex
			for (int i = 1; i <= 4; i++)
			{
				split_f = splitString(split_l[i], "/");
				faceInfo.push_back(
					XMFLOAT3(std::stof(split_f[0]), std::stof(split_f[1]), std::stof(split_f[2])));
			}
			//Set Vertices
			for (int i = 0; i < 4; i++)
			{
				vertices.push_back(CVertex(
					positions[faceInfo[i].x].x,
					positions[faceInfo[i].x].y,
					positions[faceInfo[i].x].z,
					uv[faceInfo[i].y].x,
					uv[faceInfo[i].y].y,
					normals[faceInfo[i].z].x,
					normals[faceInfo[i].z].y,
					normals[faceInfo[i].z].z));
			}
		}
	}
	for (int i = 0; i < faceCount; i++)
	{
		m_obj.indices.push_back(1 + (i * 6));
		m_obj.indices.push_back(2 + (i * 6));
		m_obj.indices.push_back(3 + (i * 6));
		m_obj.indices.push_back(1 + (i * 6));
		m_obj.indices.push_back(3 + (i * 6));
		m_obj.indices.push_back(4 + (i * 6));
	}

	m_obj.vertices = vertices;
	m_obj.vertexCount = vertexCount;
	m_obj.vertexStride = sizeof(CVertex);
	m_obj.indexCount = 6 * faceCount;
}

std::vector<std::string> CObjLoader::splitString(std::string _s, std::string _delim) {
	std::vector<std::string> list;
	size_t pos = 0;
	std::string token;

	while ((pos = _s.find(_delim)) != std::string::npos) {
		token = _s.substr(0, pos);
		list.push_back(token);
		_s.erase(0, pos + _delim.length());
	}
	list.push_back(_s);
	return list;
}