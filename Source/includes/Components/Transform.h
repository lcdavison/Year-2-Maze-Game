#pragma once

#include <glm/glm.hpp>

#include "Component.h"

struct Transform : public Component
{
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};
