#pragma once

#include <glm/glm.hpp>

/*
	The vertex data structure contains the data for each vertex required to render a model.
	This data includes the vertex position, normal, and texture coordinates.
*/
struct Vertex 
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texture_coordinate;
};
