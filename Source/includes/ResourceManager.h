#pragma once

#include "Importer.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class ResourceManager 
{
	public:
		ResourceManager (  );

		bool LoadMesh ( const char* );
		bool LoadShader ( const char*, std::string& );

	private:


};
