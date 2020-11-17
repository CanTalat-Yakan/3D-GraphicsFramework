#include "CObjLoader.h"
#include <fstream>
#include <vector>

CObj CObjLoader::load(LPCWSTR _fileName)
{
	loadFile(_fileName);

	loadVertices();
	loadIndices();

	return m_obj;
}

void CObjLoader::loadVertices()
{
	float hs = 0.5f;
	// quad - with uv & normals
	m_obj.vertices = {
		// Front Face
		CVertex(-hs, -hs, -hs, 0, 1, 0, 0, 1), //Bottom		Left
		CVertex(-hs,  hs, -hs, 0, 0, 0, 0, 1), //Top		Left
		CVertex(hs,  hs, -hs, 1, 0, 0, 0, 1), //Top		Right
		CVertex(hs, -hs, -hs, 1, 1, 0, 0, 1), //Bottom		Right

		// Left Face
		CVertex(-hs, -hs,  hs, 0, 1, 1, 0, 0), //Bottom		Left
		CVertex(-hs,  hs,  hs, 0, 0, 1, 0, 0), //Top		Left
		CVertex(-hs,  hs, -hs, 1, 0, 1, 0, 0), //Top		Right
		CVertex(-hs, -hs, -hs, 1, 1, 1, 0, 0), //Bottom		Right

		// Back Face
		CVertex(hs, -hs,  hs, 0, 1, 0, 0, -1), //Bottom	Left
		CVertex(hs,  hs,  hs, 0, 0, 0, 0, -1), //Top		Left
		CVertex(-hs,  hs,  hs, 1, 0, 0, 0, -1), //Top		Right
		CVertex(-hs, -hs,  hs, 1, 1, 0, 0, -1), //Bottom	Right

		// Right Face
		CVertex(hs, -hs, -hs, 0, 1, -1, 0, 0), //Bottom	Left
		CVertex(hs,  hs, -hs, 0, 0, -1, 0, 0), //Top		Left
		CVertex(hs,  hs,  hs, 1, 0, -1, 0, 0), //Top		Right
		CVertex(hs, -hs,  hs, 1, 1, -1, 0, 0), //Bottom	Right

		// Top Face
		CVertex(-hs,  hs, -hs, 0, 1, 0, -1, 0), //Bottom	Left
		CVertex(hs,  hs, -hs, 0, 0, 0, -1, 0), //Top		Left
		CVertex(hs,  hs,  hs, 1, 0, 0, -1, 0), //Top		Right
		CVertex(-hs,  hs,  hs, 1, 1, 0, -1, 0), //Bottom	Right

		// Bottom Face					 	
		CVertex(hs, -hs,  hs, 0, 1, 0, 1, 0), //Bottom	Left
		CVertex(-hs, -hs,  hs, 0, 0, 0, 1, 0), //Top		Left
		CVertex(-hs, -hs, -hs, 1, 0, 0, 1, 0), //Top		Right
		CVertex(hs, -hs, -hs, 1, 1, 0, 1, 0), //Bottom	Right
	};
}

void CObjLoader::loadIndices()
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
	std::vector<XMFLOAT3> positions;
	std::vector<XMFLOAT2> uv;
	std::vector<XMFLOAT3> normals;
	std::vector<XMFLOAT3> faceIndices;
	std::vector<CVertex> vertices;

	std::string line;
	while (std::getline(*_fileStream, line))
	{
		std::vector<std::string> split = splitString(line, " ");
		//Vertex
		if (split[0] == "v") 
			positions.push_back(
				XMFLOAT3(std::stof(split[1]), std::stof(split[2]), std::stof(split[3])));
		//UV
		if (split[0] == "vt") 
			uv.push_back(
				XMFLOAT2(std::stof(split[1]), std::stof(split[2])));
		//Normal
		if (split[0] == "vn") 
			normals.push_back(
				XMFLOAT3(std::stof(split[1]), std::stof(split[2]), std::stof(split[3])));
		//Face
		if (split[0] == "f")
		{
			std::vector<std::string> split_f = splitString(split[1], "/");
			faceIndices.push_back(
				XMFLOAT3(std::stof(split_f[0]), std::stof(split_f[1]), std::stof(split_f[2])));

			split_f = splitString(split[2], "/");
			faceIndices.push_back(
				XMFLOAT3(std::stof(split_f[0]), std::stof(split_f[1]), std::stof(split_f[2])));

			split_f = splitString(split[3], "/");
			faceIndices.push_back(
				XMFLOAT3(std::stof(split_f[0]), std::stof(split_f[1]), std::stof(split_f[2])));

			split_f = splitString(split[4], "/");
			faceIndices.push_back(
				XMFLOAT3(std::stof(split_f[0]), std::stof(split_f[1]), std::stof(split_f[2])));

			for (int i = 0; i < 4; i++)
			{
				vertices.push_back(CVertex(
					positions[faceIndices[i].x].x,
					positions[faceIndices[i].x].y,
					positions[faceIndices[i].x].z,
					uv[faceIndices[i].y].x,
					uv[faceIndices[i].y].y,
					normals[faceIndices[i].z].x,
					normals[faceIndices[i].z].y,
					normals[faceIndices[i].z].z));
			}
		}
	}
	m_obj.vertices = vertices;

	for (int i = 0; i < 6; i++)
	{
		m_obj.indices.push_back(1 + (i * 6));
		m_obj.indices.push_back(3 + (i * 6));
		m_obj.indices.push_back(2 + (i * 6));
		m_obj.indices.push_back(1 + (i * 6));
		m_obj.indices.push_back(2 + (i * 6));
		m_obj.indices.push_back(0 + (i * 6));
	}
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