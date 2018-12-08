#pragma once

#include <glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Vertex.h"
#include "Texture.h"
#include "ModelData.h"
#include "Components/Model.h"
#include "Mesh.h"

#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class ResourceManager 
{
	public:
		ResourceManager (  );

		bool LoadModel ( const std::string& );
		bool LoadTexture ( const std::string&, unsigned int& );
		bool LoadShader ( const char*, std::string& );

		void GetModel ( const std::string&, std::vector < unsigned int >*, std::vector < unsigned int >* );

	private:

		void ProcessNode ( aiNode*, const aiScene*, std::vector < Mesh >& );
		Mesh ProcessMesh ( aiMesh*, const aiScene* );
		void ProcessModel ( ModelData& );
		std::vector < Texture > LoadMaterial ( aiMaterial*, aiTextureType );

		TEXTURE_TYPE ConvertType ( const aiTextureType );

		std::string directory;

		// Models Loaded Into Memory
		// String = Model FileName
		std::map < std::string, ModelData > loaded_models;
};
