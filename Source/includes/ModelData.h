#pragma once

#include "Mesh.h"

/*
  ModelData stores all of the meshes that will be required to render a whole model.
*/
struct ModelData
{
	std::vector < Mesh > meshes;	//	Stores each mesh that belongs to this model
};
