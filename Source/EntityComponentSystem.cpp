#include "includes/EntityComponentSystem.h"

unsigned int EntityComponentSystem::new_id = 0;

EntityComponentSystem::EntityComponentSystem (  )
{
}

void EntityComponentSystem::CreateEntity ( std::string name )
{
	Entity new_entity;

	new_entity.id = new_id++;
	new_entity.name = name;

	if ( new_entity.name == "Entity_" )
		new_entity.name.append ( std::to_string ( new_entity.id ) );

	entities.push_back ( new_entity );
}

void EntityComponentSystem::Print (  )
{
	for ( Entity entity : entities )
	{
		std::cout << "ID : " << entity.id << " Name : " << entity.name << std::endl;
	}
}

void EntityComponentSystem::Update ( const float& delta_time )
{
	//	TODO: Update all component systems
}
