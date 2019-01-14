#pragma once

#include "Vertex.h"
#include "Texture.h"

#include <vector>

/*
  Mesh stores vertex, index, and texture data for a mesh in a model, 
  while also storing the OpenGL buffers that can be used to render this mesh.
*/
struct Mesh 
{
	std::vector < Vertex > vertices;	//	Vertex Data
	std::vector < unsigned int > indices;	//	Index Data
	std::vector < Texture > textures;	//	Texture Data

	//	OpenGL Buffer IDs
	unsigned int vertex_array, vertex_buffer, index_buffer;
};
