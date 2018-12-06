#pragma once

#include <glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Vertex.h"
#include "Texture.h"
#include "Components/Model.h"
#include "Mesh.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

//  TODO: Flyweight - Load Model in once, and let other models reference the data for rendering

class ResourceManager 
{
	public:
		ResourceManager (  );

		bool LoadModel ( const std::string&, Model* );
		bool LoadTexture ( const std::string&, unsigned int& );
		bool LoadShader ( const char*, std::string& );

	private:

		void ProcessNode ( aiNode*, const aiScene*, std::vector < Mesh >& );
		Mesh ProcessMesh ( aiMesh*, const aiScene* );
		std::vector < Texture > LoadMaterial ( aiMaterial*, aiTextureType );

		TEXTURE_TYPE ConvertType ( const aiTextureType );

		std::string directory;
};
