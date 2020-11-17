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
	void readFile(std::ifstream* _fileStream);
	std::vector<std::string> splitString(std::string _s, std::string _delim); 

	std::string m_file;
	CObj m_obj;
};

