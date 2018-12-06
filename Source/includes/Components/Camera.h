#pragma once

#include "Component.h"
#include <glm/glm.hpp>

struct Camera : public Component 
{
	glm::vec3 position;
	glm::vec3 rotation;
};
