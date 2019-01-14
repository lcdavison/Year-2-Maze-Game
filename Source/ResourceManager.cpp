#include "Includes/ResourceManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "includes/STB/stb_image.h"

ResourceManager::ResourceManager (  )
{
}

/*
	LoadModel

	Loads model data from the file provided.
*/
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

	// Generate OpenGL Buffers for ModelData
	ProcessModel ( model );

	loaded_models [ filename ] = model;

	return true;	
}

/*
	ProcessNode

	Processes a node from the Assimp model loader, and stores the meshes in the provided vector.
*/
void ResourceManager::ProcessNode ( aiNode* node, const aiScene* scene, std::vector < Mesh >& meshes )
{
	for ( int i = 0; i < node->mNumMeshes; i++ )
	{
		aiMesh* mesh = scene->mMeshes [ node->mMeshes [ i ] ];
		meshes.push_back ( ProcessMesh ( mesh, scene ) );
	}

	for ( int i = 0; i < node->mNumChildren; i++ )
	{
		ProcessNode ( node->mChildren [ i ], scene, meshes );
	}
}

/*
	ProcessMesh

	Processes the data in a mesh from the Assimp model loader,
	this includes vertices, vertex normals, and texture_coordinates.
*/
Mesh ResourceManager::ProcessMesh ( aiMesh* mesh, const aiScene* scene )
{
	Mesh new_mesh;

	//	Process each vertex
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

		// Check if mesh has texture coordinates
		if ( mesh->mTextureCoords [ 0 ] != NULL )
		{
			//	Texture Coordinates
			vertex.texture_coordinate.x = mesh->mTextureCoords [ 0 ] [ i ].x;
			vertex.texture_coordinate.y = mesh->mTextureCoords [ 0 ] [ i ].y;
		}

		//	Add to processed mesh data
		new_mesh.vertices.push_back ( vertex );
	}

	//	Process each face
	for ( int i = 0; i < mesh->mNumFaces; i++ )
	{
		aiFace face = mesh->mFaces [ i ];

		//	Process indices for the face
		for ( int j = 0; j < face.mNumIndices; j++ )
		{
			new_mesh.indices.push_back ( face.mIndices [ j ] );
		}
	}

	//	Determine if materials are used
	if ( mesh->mMaterialIndex >= 0 )
	{
		//	Retrieve materials
		aiMaterial* material = scene->mMaterials [ mesh->mMaterialIndex ];

		//	Load diffuse textures
		std::vector < Texture > diffuse_maps = LoadMaterial ( material, aiTextureType_DIFFUSE );
		new_mesh.textures.insert ( new_mesh.textures.end (  ), diffuse_maps.begin (  ), diffuse_maps.end (  ) );

		//	Load speculur textures
		std::vector < Texture > specular_maps = LoadMaterial ( material, aiTextureType_SPECULAR );
		new_mesh.textures.insert ( new_mesh.textures.end (  ), specular_maps.begin (  ), specular_maps.end (  ) );
	}

	return new_mesh;
}

/*
	LoadMaterial


*/
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

/*
	ProcessModel

	Processes the provided model data, and buffers it on the GPU.
*/
void ResourceManager::ProcessModel ( ModelData& model )
{
	//	Process each mesh in the model
	for ( Mesh& mesh : model.meshes )
	{
		//	Generate a vertex array
		glGenVertexArrays ( 1, &mesh.vertex_array );
		glBindVertexArray ( mesh.vertex_array );

		//	Generate a vertex buffer
		glGenBuffers ( 1, &mesh.vertex_buffer );
		glBindBuffer ( GL_ARRAY_BUFFER, mesh.vertex_buffer );
		glBufferData ( GL_ARRAY_BUFFER, mesh.vertices.size (  ) * sizeof ( Vertex ), &mesh.vertices [ 0 ], GL_STATIC_DRAW );

		//	Split each vertex in the buffer into three chunks
		glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, sizeof ( Vertex ), ( void* ) 0 );
		glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE, sizeof ( Vertex ), ( void* ) offsetof ( Vertex, normal ) );
		glVertexAttribPointer ( 2, 2, GL_FLOAT, GL_FALSE, sizeof ( Vertex ), ( void* ) offsetof ( Vertex, texture_coordinate ) );

		//	Generate an index buffer
		glGenBuffers ( 1, &mesh.index_buffer );
		glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, mesh.index_buffer );
		glBufferData ( GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size (  ) * sizeof ( unsigned int ), &mesh.indices [ 0 ], GL_STATIC_DRAW );

		//	Unbind buffers and vertex array
		glBindBuffer ( GL_ARRAY_BUFFER, 0 );
		glBindVertexArray ( 0 );
	}
}

/*
	ConvertType

	Used to convert an Assimp texture type to a type native to the game engine.
*/
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

/*
	LoadShader

	Loads shader code from the supplied file path, and returns the shader
	code in out_shader.
*/
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

/*
	LoadTexture

	Loads a texture from the supplied file path, and returns the texture ID
	in out_texture.
*/
bool ResourceManager::LoadTexture ( const std::string& path, unsigned int& out_texture )
{
	//	Generate texture
	glGenTextures ( 1, &out_texture );

	//	Load texture data
	int width, height, nrComponents;
	unsigned char* data = stbi_load ( path.c_str (  ), &width, &height, &nrComponents, 0 );

	//	Check data is valid
	if ( data == NULL )
	{
		//	Fail and free data
		std::cout << "Failed to load texture" << std::endl;
		stbi_image_free ( data );

		return false;
	}
	else 
	{
		//	Determine format of texture
		GLenum format;
		if ( nrComponents == 1 )
			format = GL_RED;
		else if ( nrComponents == 3 )
			format = GL_RGB;
		else if ( nrComponents == 4 )
			format = GL_RGBA;

		//	Bind texture
		glBindTexture ( GL_TEXTURE_2D, out_texture );

		//	Send texture data to GPU
		glTexImage2D ( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data );
		
		//	Generate a mipmap for the texture
		glGenerateMipmap ( GL_TEXTURE_2D );

		//	Set Texture Parameters
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		//	Free texture data
		stbi_image_free ( data );

		return true;
	}
}

/*
	GetModel

	Returns the model associated with the filename
*/
const ModelData ResourceManager::GetModel ( const std::string& filename )
{
	return loaded_models [ filename ];
}
