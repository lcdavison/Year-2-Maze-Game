#include "includes/EntityComponentSystem.h"

unsigned int EntityComponentSystem::new_id = 0;

//	TODO: Create an entity pool

EntityComponentSystem::EntityComponentSystem (  )
{
}

EntityComponentSystem::~EntityComponentSystem (  )
{
	Clear (  );

	/*
	if ( models.size (  ) > 0 )
	ClearModels (  );*/
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

	entities.at ( entity->id ).component_mask |= TRANSFORM;

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

Model* EntityComponentSystem::AddModel ( const Entity* entity, const std::string& filename )
{
	Model new_model;

	new_model.entity = entity->id;
	new_model.filename = filename;
	
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

	std::cout << entity->component_mask << std::endl;

	if ( !( entity->component_mask & TRANSFORM ) )
	{
		Transform* transform = AddTransform ( entity );
		transform->position = glm::vec3 ( 0, 1, 0 );
	}
}

void EntityComponentSystem::AddPlayerCommand ( const SDL_Keycode& key, void ( *action ) (  ), const COMMAND_TYPE& type )
{
	Command new_command;

	new_command.key = key;	
	new_command.Action = action;
	new_command.type = type;

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
	// Iterate Each Component List and Clear it
}

/*
void EntityComponentSystem::ClearModels (  )
{
	// Delete the data for each model
	for ( Model& model : models )
	{
		// Delete the buffers for each mesh
		for ( Mesh& mesh : model.meshes )
		{
			glDeleteBuffers ( 1, &mesh.vertex_buffer );
			glDeleteBuffers ( 1, &mesh.index_buffer );
			glDeleteVertexArrays ( 1, &mesh.vertex_array );
		}
	}
	}*/
