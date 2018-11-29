#pragma once

#include "Importer.hpp"

#include <fstream>

class ResourceManager 
{
	public:
		ResourceManager (  );

		bool LoadMesh ( const char* );
		bool LoadShader ( const char*, char* );

	private:


};
