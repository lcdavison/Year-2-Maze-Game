#pragma once

#include <glm/glm.hpp>

#include "Component.h"

/*
	The transform component contains all the data required to manipulate
	the properties of a mesh in the game world.
*/
struct Transform : public Component
{
	//	Transformation data
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	//	Forward vector used for movement
	glm::vec3 forward;

	//	Local axes
	glm::vec3 local_x_axis;
	glm::vec3 local_y_axis;
	glm::vec3 local_z_axis;
};
