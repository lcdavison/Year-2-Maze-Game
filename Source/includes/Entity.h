#pragma once

#include <string>

enum COMPONENT { TRANSFORM = 0x1, MODEL = 0x2, PHYSICS = 0x4 };		//	Stores identifiers for components, used with the component mask to indicate the components attached to this entity

/*
	Entity contains data related to each object that can be placed in the world.
*/
struct Entity 
{
	unsigned int id;	//	The unique identifier of the entity
        std::string name;	//	The name of the entity

	unsigned int component_mask = 0x0;	//	The component mask used to identify attached components quickly
};
