#pragma once

#include "Component.h"

#include "Command.h"

#include <vector>

/*
	The player controller component stores a set of commands used to update the player.
*/
struct PlayerController : public Component 
{
	std::vector < Command > commands;
};
