#include "includes/EntityComponentSystem.h"

unsigned int EntityComponentSystem::new_id = 0;

EntityComponentSystem::EntityComponentSystem (  )
{
}

const Entity* EntityComponentSystem::CreateEntity ( std::string name )
{
	Entity new_entity;

	new_entity.id = new_id++;
	new_entity.name = name;

	if ( new_entity.name == "Entity_" )
		new_entity.name.append ( std::to_string ( new_entity.id ) );

	entities.push_back ( new_entity );

	return &entities.back (  );
}

const Entity* EntityComponentSystem::GetEntity ( const unsigned int& id )
{
	for ( int i = 0; i < entities.size (  ); i++ )	
	{
		if ( entities.at ( i ).id == id )
		{
			return &entities.at ( i );
		}
	}
}

Transform* EntityComponentSystem::AddTransform ( const Entity* entity )
{
	Transform new_transform;

	new_transform.entity = entity->id;

	new_transform.position = glm::vec3 ( 0 );
	new_transform.rotation = glm::vec3 ( 0 );
	new_transform.scale = glm::vec3 ( 1 );

	transforms.push_back ( new_transform );

	return &transforms.back (  );
}

Transform* EntityComponentSystem::GetTransform ( const Entity* entity )
{
	for ( int i = 0; i < transforms.size (  ); i++ )
	{
		if ( transforms.at ( i ).entity == entity->id )
			return &transforms [ i ];
	}

	return nullptr;
}

std::vector < Transform >& EntityComponentSystem::GetTransforms (  )
{
	return transforms;
}

Model* EntityComponentSystem::AddModel ( const Entity* entity )
{
	Model new_model;

	new_model.entity = entity->id;

	models.push_back ( new_model );

	return &models.back (  );
}

Model* EntityComponentSystem::GetModel ( const Entity* entity )
{
	for ( int i = 0; i < models.size (  ); i++ )
	{
		if ( models.at ( i ).entity == entity->id )
			return &models [ i ];
	}

	return nullptr;
}

std::vector < Model >& EntityComponentSystem::GetModels (  )
{
	return models;
}

void EntityComponentSystem::SetPlayer ( const Entity* entity )
{
	player_controller.entity = entity->id;
}

void EntityComponentSystem::AddPlayerCommand ( const SDL_Keycode& key, void ( *action ) (  ), const COMMAND_TYPE& type )
{
	Command new_command;

	new_command.key = key;	

	switch ( type )
	{
		case KEYPRESS:
			new_command.OnKeyDown = action;
			break;
		case KEYRELEASE:
			new_command.OnKeyUp = action;
			break;
	}

	player_controller.commands.push_back ( new_command );
}

const PlayerController* EntityComponentSystem::GetPlayerController (  )
{
	return &player_controller;
}

void EntityComponentSystem::Update ( const float& delta_time )
{
	//	TODO: Update all component systems
}

void EntityComponentSystem::Clear (  )
{
	// TODO: Clear all of the components and entities
}

void EntityComponentSystem::ClearModels (  )
{
	// TODO: Clear all the model buffers
}
