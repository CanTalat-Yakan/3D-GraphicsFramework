#include "CObjLoader.h"
#include <fstream>

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
	std::ifstream input(_fileName);
	if (!input.is_open())
		return;

	std::string line;
	while (std::getline(input, line))
		m_file.append(line.c_str());

	input.close();
}
