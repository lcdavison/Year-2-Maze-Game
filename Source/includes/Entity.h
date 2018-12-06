#pragma once

#include <string>

enum COMPONENT { TRANSFORM = 0x1, MODEL = 0x2 };

struct Entity 
{
	unsigned int id;
        std::string name;

	unsigned int component_mask = 0x0;
};
