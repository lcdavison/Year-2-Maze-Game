#include "includes/EntityComponentSystem.h"

unsigned int EntityComponentSystem::new_id = 0;

EntityComponentSystem::EntityComponentSystem (  )
{
}

Entity EntityComponentSystem::CreateEntity ( std::string name )
{
	Entity new_entity;

	new_entity.id = new_id++;
	new_entity.name = name;

	if ( new_entity.name == "Entity_" )
		new_entity.name.append ( std::to_string ( new_entity.id ) );

	entities.push_back ( new_entity );

	return new_entity;
}

Transform EntityComponentSystem::AddTransform ( const Entity& entity )
{
	Transform new_transform;

	new_transform.entity = entity.id;

	new_transform.position = glm::vec3 ( 0 );
	new_transform.rotation = glm::vec3 ( 0 );
	new_transform.scale = glm::vec3 ( 1 );

	transforms.push_back ( new_transform );

	return new_transform;
}

Transform EntityComponentSystem::GetTransform ( const Entity& entity )
{
	std::vector < Component > search ( transforms.begin (  ), transforms.end (  ) );
	unsigned int index = BinarySearch ( search, entity );

	return transforms [ index ];
}

Model EntityComponentSystem::AddModel ( const Entity& entity )
{
	Model new_model;

	new_model.entity = entity.id;

	models.push_back ( new_model );

	return new_model;
}

Model EntityComponentSystem::GetModel ( const Entity& entity )
{
	std::vector < Component > search ( models.begin (  ), models.end (  ) );
	unsigned int index = BinarySearch ( search, entity );

	return models [ index ];
}

void EntityComponentSystem::Update ( const float& delta_time )
{
	//	TODO: Update all component systems
}

unsigned int EntityComponentSystem::BinarySearch ( std::vector < Component >& components, const Entity& entity )
{
	unsigned int start = 0, end = components.size (  ) - 1;

	while ( start < end )
	{
		unsigned int mid = ( start + ( end - start ) ) / 2;

		if ( components [ mid ].entity == entity.id )
			return mid;
		else if ( components [ mid ].entity < entity.id )
			start = mid + 1;
		else if ( components [ mid ].entity > entity.id )
			end = mid - 1;
	}
}
