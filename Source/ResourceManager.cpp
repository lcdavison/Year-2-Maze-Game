#include "Includes/ResourceManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "includes/STB/stb_image.h"

ResourceManager::ResourceManager (  )
{
}

bool ResourceManager::LoadModel ( const std::string& path )
{
	Assimp::Importer importer;
	ModelData model;

	const aiScene* scene = importer.ReadFile ( path.c_str (  ), aiProcess_Triangulate | aiProcess_FlipUVs );

	if ( scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr )
	{
		std::cout << importer.GetErrorString (  ) << std::endl;
		return false;
	}

	directory = path.substr ( 0, path.find_last_of ( '/' ) );

	ProcessNode ( scene->mRootNode, scene, model.meshes );

	std::string filename = path.substr ( path.find_last_of ( '/' ) + 1, path.back (  ) );

	std::cout << "Model Loaded : " << filename << std::endl;

	// Generate OpenGL Buffers for ModelData
	ProcessModel ( model );

	loaded_models [ filename ] = model;

	return true;	
}

void ResourceManager::ProcessNode ( aiNode* node, const aiScene* scene, std::vector < Mesh >& meshes )
{
	for ( int i = 0; i < node->mNumMeshes; i++ )
	{
		aiMesh* mesh = scene->mMeshes [ node->mMeshes [ i ] ];
		meshes.push_back ( ProcessMesh ( mesh, scene ) );
	}

	for ( int i = 0; i < node->mNumChildren; i++ )
	{
		std::cout << "Child" << std::endl;
		ProcessNode ( node->mChildren [ i ], scene, meshes );
	}

	std::cout << "Node Processed" << std::endl;
}

Mesh ResourceManager::ProcessMesh ( aiMesh* mesh, const aiScene* scene )
{
	Mesh new_mesh;

	for ( int i = 0; i < mesh->mNumVertices; i++ )
	{
		Vertex vertex;

		//	Position
		vertex.position.x = mesh->mVertices [ i ].x;
		vertex.position.y = mesh->mVertices [ i ].y;
		vertex.position.z = mesh->mVertices [ i ].z;

		//	Normals
		vertex.normal.x = mesh->mNormals [ i ].x;
		vertex.normal.y = mesh->mNormals [ i ].y;
		vertex.normal.z = mesh->mNormals [ i ].z;

		//	Texture Coordinates
		//vertex.texture_coordinate.x = mesh->mTextureCoords [ i ]->x;
		//vertex.texture_coordinate.y = mesh->mTextureCoords [ i ]->y;

		new_mesh.vertices.push_back ( vertex );
	}

	for ( int i = 0; i < mesh->mNumFaces; i++ )
	{
		aiFace face = mesh->mFaces [ i ];

		for ( int j = 0; j < face.mNumIndices; j++ )
		{
			new_mesh.indices.push_back ( face.mIndices [ j ] );
		}
	}

	if ( mesh->mMaterialIndex >= 0 )
	{
		aiMaterial* material = scene->mMaterials [ mesh->mMaterialIndex ];

		std::vector < Texture > diffuse_maps = LoadMaterial ( material, aiTextureType_DIFFUSE );
		new_mesh.textures.insert ( new_mesh.textures.end (  ), diffuse_maps.begin (  ), diffuse_maps.end (  ) );

		std::vector < Texture > specular_maps = LoadMaterial ( material, aiTextureType_SPECULAR );
		new_mesh.textures.insert ( new_mesh.textures.end (  ), specular_maps.begin (  ), specular_maps.end (  ) );
	}

	return new_mesh;
}

std::vector < Texture > ResourceManager::LoadMaterial ( aiMaterial* material, aiTextureType type )
{
	std::vector < Texture > textures;

	for ( int i = 0; i < material->GetTextureCount ( type ); i++ )
	{
		aiString file;
		material->GetTexture ( type, i, &file );

		Texture texture;

		std::string filename = std::string ( file.C_Str (  ) );
		LoadTexture ( directory + "/" + filename , texture.texture_id );
		
		texture.type = ConvertType ( type );

		textures.push_back ( texture );
	}

	return textures;
}

void ResourceManager::ProcessModel ( ModelData& model )
{
	for ( Mesh& mesh : model.meshes )
	{
		glGenVertexArrays ( 1, &mesh.vertex_array );
		glBindVertexArray ( mesh.vertex_array );

		glGenBuffers ( 1, &mesh.vertex_buffer );
		glBindBuffer ( GL_ARRAY_BUFFER, mesh.vertex_buffer );
		glBufferData ( GL_ARRAY_BUFFER, mesh.vertices.size (  ) * sizeof ( Vertex ), &mesh.vertices [ 0 ], GL_STATIC_DRAW );

		glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, sizeof ( Vertex ), ( void* ) 0 );
		glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE, sizeof ( Vertex ), ( void* ) offsetof ( Vertex, normal ) );
		//glVertexAttribPointer ( 2, 2, GL_FLOAT, GL_FALSE, sizeof ( Vertex ), ( void* ) offsetof ( Vertex, texture_coordinates ) );

		glGenBuffers ( 1, &mesh.index_buffer );
		glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, mesh.index_buffer );
		glBufferData ( GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size (  ) * sizeof ( unsigned int ), &mesh.indices [ 0 ], GL_STATIC_DRAW );

		glBindBuffer ( GL_ARRAY_BUFFER, 0 );
		glBindVertexArray ( 0 );
	}
}

TEXTURE_TYPE ResourceManager::ConvertType ( const aiTextureType type )
{
	switch ( type )	
	{
		case aiTextureType_DIFFUSE:
			return DIFFUSE;
		case aiTextureType_SPECULAR:
			return SPECULAR;
	}
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
		stringbuffer << line << std::endl;
	}

	file.close (  );

	out_shader = stringbuffer.str (  );

	return true;
}

bool ResourceManager::LoadTexture ( const std::string& path, unsigned int& out_texture )
{
	glGenTextures ( 1, &out_texture );

	int width, height, nrComponents;
	unsigned char* data = stbi_load ( path.c_str (  ), &width, &height, &nrComponents, 0 );
	
	if ( data == NULL )
	{
		std::cout << "Failed to load texture" << std::endl;
		stbi_image_free ( data );

		return false;
	}
	else 
	{
		GLenum format;
		if ( nrComponents == 1 )
			format = GL_RED;
		else if ( nrComponents == 3 )
			format = GL_RGB;
		else if ( nrComponents == 4 )
			format = GL_RGBA;

		glBindTexture ( GL_TEXTURE_2D, out_texture );
		glTexImage2D ( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data );
		glGenerateMipmap ( GL_TEXTURE_2D );

		//	Set Texture Parameters
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		stbi_image_free ( data );

		return true;
	}
}

void ResourceManager::GetModel ( const std::string& filename, std::vector < unsigned int >* vertex_arrays, std::vector < unsigned int >* vertex_counts )
{	
	for ( Mesh mesh : loaded_models [ filename ].meshes )
	{
		vertex_arrays->push_back ( mesh.vertex_array );
		vertex_counts->push_back ( mesh.indices.size (  ) );
	}
}
