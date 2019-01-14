#pragma once

#include "Component.h"

#include <glm/glm.hpp>

/*
	The physics component contains the velocity of an entity.
*/
struct Physics : public Component
{
	glm::vec3 velocity;
};
