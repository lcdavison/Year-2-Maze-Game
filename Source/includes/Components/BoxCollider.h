#pragma once

#include "Component.h"

#include <glm/glm.hpp>

/*
	The box collider component contains data that can be used to test for collisions.
*/
struct BoxCollider : public Component
{
	//	Local extents
	glm::vec4 min_extents;
	glm::vec4 max_extents;
	glm::vec4 local_center;

	//	World extents
	glm::vec4 min;
	glm::vec4 max;
	glm::vec4 center;

	//	Collider size
	glm::vec3 size;
};
