#pragma once

#include "Mesh.h"

#include "Component.h"

struct Model : public Component 
{
	std::vector < Mesh > meshes;
};
