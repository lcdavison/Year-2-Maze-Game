#include "Includes/ResourceManager.h"

ResourceManager::ResourceManager (  )
{
}

bool ResourceManager::LoadMesh ( const char* path )
{
}

bool ResourceManager::LoadShader ( const char* path, std::string& out_shader )
{
	std::ifstream file;

	file.open ( path );

	if ( !file.is_open (  ) )
	{
		std::cout << "Failed to open file : " << path << std::endl;
		return false;
	}

	std::string line;
	std::stringstream stringbuffer;

	while ( std::getline ( file, line ) )
	{
		stringbuffer << line;
	}

	file.close (  );

	out_shader = stringbuffer.str (  );

	return true;
}
