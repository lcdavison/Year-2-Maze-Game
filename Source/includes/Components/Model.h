#pragma once

#include "Component.h"

#include <vector>
#include <string>

/*
	The model component contains the filename of the model
	that should be rendered in the game.
*/
struct Model : public Component 
{
	std::string filename;
};
