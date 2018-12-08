#pragma once

#include "Component.h"

#include <vector>
#include <string>

struct Model : public Component 
{
	std::string filename;
	
	std::vector < unsigned int > num_vertices;
	std::vector < unsigned int > vertex_arrays;
};
