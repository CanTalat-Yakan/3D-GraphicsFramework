#pragma once
#include "CObj.h"
#include <string>
#include <d3d11.h>

class CObjLoader
{
public:
	CObj load(LPCWSTR _fileName);

private:
	void loadVertices();
	void loadIndices();
	void loadFile(LPCWSTR _fileName);

	std::string m_file;
	CObj m_obj;
};

