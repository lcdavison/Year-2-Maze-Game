#pragma once

#include "Vertex.h"

#include <vector>

struct Mesh 
{
	std::vector < Vertex > vertices;
	std::vector < unsigned int > indices;
	std::vector < Texture > textures;

	unsigned int vertex_array, vertex_buffer, index_buffer;
};
