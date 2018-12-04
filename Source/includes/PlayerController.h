#pragma once

#include "Component.h"

#include "Command.h"

#include <vector>

struct PlayerController : public Component 
{
	std::vector < Command > commands;
};
