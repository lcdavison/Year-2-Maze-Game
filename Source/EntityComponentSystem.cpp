#include "includes/EntityComponentSystem.h"

unsigned int EntityComponentSystem::new_id = 0;		//	Stores the next ID for a new entity

EntityComponentSystem::EntityComponentSystem (  )
{
}

EntityComponentSystem::~EntityComponentSystem (  )
{
	Clear (  );
}

/*
	CreateEntity

	Creates an entity with the specified name.
*/
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

/*
	GetEntity

	Searches for an entity with the specified ID.
*/
const Entity* EntityComponentSystem::GetEntity ( unsigned int id )
{
	for ( int i = 0; i < entities.size (  ); i++ )	
	{
		if ( entities.at ( i ).id == id )
		{
			return &entities.at ( i );
		}
	}
}

/*
	AddTransform

	Adds a transform component to the specified entity.
*/
Transform* EntityComponentSystem::AddTransform ( const Entity* entity )
{
	Transform new_transform;

	new_transform.entity = entity->id;

	//	Set Component Mask
	entities.at ( entity->id ).component_mask |= TRANSFORM;

	//	Set Default Values
	new_transform.position = glm::vec3 ( 0 );
	new_transform.rotation = glm::vec3 ( 0 );
	new_transform.scale = glm::vec3 ( 1 );

	new_transform.forward = glm::vec3 ( 0, 0, 1 );

	transforms.push_back ( new_transform );

	return &transforms.back (  );
}

/*
	GetTransform

	Searches for a transform via the specified entity ID.
*/
Transform* EntityComponentSystem::GetTransform ( unsigned int entity_id )
{
	for ( int i = 0; i < transforms.size (  ); i++ )
	{
		if ( transforms.at ( i ).entity == entity_id )
			return &transforms.at ( i );
	}

	return nullptr;
}

/*
	GetTransforms

	Returns a reference to the transforms container.
*/
std::vector < Transform >& EntityComponentSystem::GetTransforms (  )
{
	return transforms;
}

/*
	AddModel

	Adds a model to the specified entity.
*/
Model* EntityComponentSystem::AddModel ( const Entity* entity, const std::string& filename )
{
	Model new_model;

	entities.at ( entity->id ).component_mask |= MODEL;

	new_model.entity = entity->id;
	new_model.filename = filename;
	
	models.push_back ( new_model );

	return &models.back (  );
}

/*
	GetModel

	Searches for a model via the specified entity ID.
*/
Model* EntityComponentSystem::GetModel ( unsigned int entity_id )
{
	for ( int i = 0; i < models.size (  ); i++ )
	{
		if ( models.at ( i ).entity == entity_id )
			return &models [ i ];
	}

	return nullptr;
}

/*
	GetModels

	Returns a reference to the models container.
*/
std::vector < Model >& EntityComponentSystem::GetModels (  )
{
	return models;
}

/*
	AddPhysics

	Adds a physics component to the supplied entity.
*/
Physics* EntityComponentSystem::AddPhysics ( const Entity* entity )
{
	Physics physic;

	entities.at ( entity->id ).component_mask |= PHYSICS;
	physic.entity = entity->id;
	physic.velocity = glm::vec3 ( 0.0f );

	physics.push_back ( physic );

	return &physics.back (  );
}

/*
	GetPhysics

	Searches for a physics component via a specified entity ID.
*/
Physics* EntityComponentSystem::GetPhysics ( unsigned int entity_id )
{
	for ( int i = 0; i < physics.size (  ); i++ )
	{
		if ( physics.at ( i ).entity == entity_id )
			return &physics.at ( i );
	}

	return nullptr;
}

/*
	AddBoxCollider

	Adds a box collider to an entity with the specified minimum and maximum extents.
*/
BoxCollider* EntityComponentSystem::AddBoxCollider ( const Entity* entity, const glm::vec3& min, const glm::vec3& max )
{
	BoxCollider collider;

	collider.entity = entity->id;

	collider.min_extents = glm::vec4 ( min, 1.0f );
	collider.max_extents = glm::vec4 ( max, 1.0f );

	box_colliders.push_back ( collider );

	return &box_colliders.back (  );
}

/*
	AddBoxCollider
	
	Adds a box collider to an entity, this uses model data to calculate 
	the minimum and maximum extents of the bounding volume.
*/
BoxCollider* EntityComponentSystem::AddBoxCollider ( const Entity* entity, const ModelData&& model )
{
	BoxCollider collider;

	collider.entity = entity->id;

	collider.min_extents.x = collider.max_extents.x = model.meshes [ 0 ].vertices [ 0 ].position.x;
	collider.min_extents.y = collider.max_extents.y = model.meshes [ 0 ].vertices [ 0 ].position.y;
	collider.min_extents.z = collider.max_extents.z = model.meshes [ 0 ].vertices [ 0 ].position.z;

	//	Calculate extents of the bounding box
	if ( entity->component_mask & MODEL )
	{
		for ( Mesh mesh : model.meshes )
		{
			for ( Vertex vertex : mesh.vertices )
			{
				if ( vertex.position.x < collider.min_extents.x ) collider.min_extents.x = vertex.position.x;
				if ( vertex.position.x > collider.max_extents.x ) collider.max_extents.x = vertex.position.x;

				if ( vertex.position.y < collider.min_extents.y ) collider.min_extents.y = vertex.position.y;
				if ( vertex.position.y > collider.max_extents.y ) collider.max_extents.y = vertex.position.y;

				if ( vertex.position.z < collider.min_extents.z ) collider.min_extents.z = vertex.position.z;
				if ( vertex.position.z > collider.max_extents.z ) collider.max_extents.z = vertex.position.z;
			}
		}
	}

	box_colliders.push_back ( collider );

	return &box_colliders.back (  );
}

/*
	GetBoxCollider
	
	Searches for a box collider via a specified entity ID
*/
BoxCollider* EntityComponentSystem::GetBoxCollider ( unsigned int entity_id )
{
	for ( int i = 0; i < box_colliders.size (  ); i++ )
	{
		if ( box_colliders.at ( i ).entity == entity_id )
			return &box_colliders.at ( i );
	}
}

/*
	GetBoxColliders

	Returns a reference to the BoxColliders container.
*/
std::vector < BoxCollider >& EntityComponentSystem::GetBoxColliders (  )
{
	return box_colliders;
}

/*
	SetPlayer

	Sets the entity that is to be the player entity.
*/
void EntityComponentSystem::SetPlayer ( const Entity* entity )
{
	player_controller.entity = entity->id;

	//	Test for missing components and add them if necessary
	if ( !( entity->component_mask & TRANSFORM ) )
	{
		Transform* transform = AddTransform ( entity );
		transform->position = glm::vec3 ( 0, 1, 0 );
	}

	if ( !( entity->component_mask & PHYSICS ) )
	{
		AddPhysics ( entity );
	}
}

/*
	AddPlayerCommand

	Adds a new command to the player controller.
*/
void EntityComponentSystem::AddPlayerCommand ( const SDL_Keycode& key, void ( *action ) (  ), const COMMAND_TYPE& type )
{
	Command new_command;

	new_command.key = key;	
	new_command.Action = action;
	new_command.type = type;

	player_controller.commands.push_back ( new_command );
}

/*
	GetPlayerController

	Returns the controller for the player.
*/
const PlayerController* EntityComponentSystem::GetPlayerController (  )
{
	return &player_controller;
}

/*
	Update

	Updates the entities, and handles collision response.
*/
void EntityComponentSystem::Update ( const float& delta_time )
{
	//	Retrieve player collider - Player is always entity 0
	BoxCollider* player_collider = GetBoxCollider ( 0 );

	//	Test For Collisions with player
	for ( BoxCollider collider : box_colliders )	
	{
		//	Skip the player collider
		if ( collider.entity == 0 )
			continue;

		CollisionManifold manifold;

		//	Discrete Collision Detection and Resolution
		if ( CheckBoxCollision ( *( player_collider ), collider, manifold ) )
		{
			//	Get player physics
			Physics* physic = GetPhysics ( player_collider->entity );

			//	Calculate correction vector
			glm::vec3 correction = ( ( 60.0f * manifold.penetration ) * manifold.normal ) + manifold.penetration;

			//	Backstab To Resolve - Not very effective
			if ( glm::dot ( physic->velocity, manifold.normal ) < 0.0f )
			{
				physic->velocity.x -= correction.x * 0.8f;
				physic->velocity.z -= correction.z * 0.8f;
			}
			else if ( glm::dot ( physic->velocity, manifold.normal ) > 0.0f )
			{
				physic->velocity.x += correction.x * 0.8f;
				physic->velocity.z += correction.z * 0.8f;
			}
		}
	}

	//	Apply velocity
	for ( Transform& transform : transforms )
	{
		Physics* physic = GetPhysics ( transform.entity );

		if ( !physic )
			continue;

		transform.position += physic->velocity * delta_time;
	}

	for ( Physics& p : physics )
	{
		//	Reset velocity
		p.velocity = glm::vec3 ( 0.0f );
	}

	//	Update Colliders
	for ( BoxCollider& collider : box_colliders )
	{
		Transform* transform = GetTransform ( collider.entity );

		glm::mat4 transformation = glm::mat4 ( 1.0 ); 

		//	Calculate collider size and center
		collider.size = glm::vec3 ( collider.max_extents.x - collider.min_extents.x, collider.max_extents.y - collider.min_extents.y, collider.max_extents.z - collider.min_extents.z );
		collider.local_center = glm::vec4 ( ( collider.min_extents.x + collider.max_extents.x ) * 0.5f, ( collider.min_extents.y + collider.max_extents.y ) * 0.5f, ( collider.min_extents.z + collider.max_extents.z ) * 0.5f, 1.0f );

		//	Create Transformation For Collider Points
		transformation = glm::translate ( transformation, transform->position ); 
		transformation = glm::rotate ( transformation, glm::radians ( transform->rotation.y ), glm::vec3 ( 0, 1, 0 ) );
		transformation = glm::rotate ( transformation, glm::radians ( 0.0f ), glm::vec3 ( 1, 0, 0 ) );
		transformation = glm::scale ( transformation, transform->scale );

		//	Extract the local axes
		transform->local_x_axis = glm::vec3 ( transformation [ 0 ] [ 0 ], transformation [ 0 ] [ 1 ], transformation [ 0 ] [ 2 ] );
		transform->local_y_axis = glm::vec3 ( transformation [ 1 ] [ 0 ], transformation [ 1 ] [ 1 ], transformation [ 1 ] [ 2 ] );
		transform->local_z_axis = glm::vec3 ( transformation [ 2 ] [ 0 ], transformation [ 2 ] [ 1 ], transformation [ 2 ] [ 2 ] );

		//	Translate and scale to fit object 
		transformation = transformation * glm::translate ( glm::mat4 ( 1.0f ), glm::vec3 ( collider.local_center ) ) * glm::scale ( glm::mat4 ( 1.0f ), collider.size );

		//	Update collider extents
		collider.min = transformation * collider.min_extents;
		collider.max = transformation * collider.max_extents;
		collider.center = transformation * collider.local_center;
	}
}

/*
   Clear

   Clears the components and entities stored in the Entity-Component System
   */
void EntityComponentSystem::Clear (  )
{
	// Clear all of the components and entities
	transforms.clear (  );
	models.clear (  );
	physics.clear (  );
	box_colliders.clear (  );

	entities.clear (  );
}

/*
   CheckBoxCollision

   Checks for a collision between two Oriented Bounding Boxes,
   and returns a manifold containing collision data. 
   Early out if a separting axis is detected, this saves some computation.
*/
bool EntityComponentSystem::CheckBoxCollision ( const BoxCollider& collider_one, const BoxCollider& collider_two, CollisionManifold& manifold )
{
	//	Oriented Bounding Boxes
	Transform* t_one = GetTransform ( collider_one.entity );
	Transform* t_two = GetTransform ( collider_two.entity );

	//	Calculate distance between centers
	glm::vec3 distance = glm::vec3 ( collider_two.center - collider_one.center );

	//	Calculate half dimensions of collider one
	float half_x_one = collider_one.size.x * 0.5f;
	float half_y_one = collider_one.size.y * 0.5f;
	float half_z_one = collider_one.size.z * 0.5f;

	//	Calculate half dimensions of collider two
	float half_x_two = collider_two.size.x * 0.5f;	
	float half_y_two = collider_two.size.y * 0.5f;
	float half_z_two = collider_two.size.z * 0.5f;

	manifold.penetration = -FLT_MAX;

	//	Project onto local axes of collider one 
	float one_x = std::abs ( glm::dot ( distance, t_one->local_x_axis ) ) - ( half_x_one 
			+ std::abs ( half_x_two * glm::dot ( t_one->local_x_axis, t_two->local_x_axis ) ) 
			+ std::abs ( half_y_two * glm::dot ( t_one->local_x_axis, t_two->local_y_axis ) ) 
			+ std::abs ( half_z_two * glm::dot ( t_one->local_x_axis, t_two->local_z_axis ) ) );

	if ( one_x > manifold.penetration )
	{
		manifold.penetration = one_x;
		manifold.normal = t_one->local_x_axis;
	}

	if ( one_x > 0.0f ) return false;

	float one_y = std::abs ( glm::dot ( distance, t_one->local_y_axis ) ) - ( half_y_one 
			+ std::abs ( half_x_two * glm::dot ( t_one->local_y_axis, t_two->local_x_axis ) ) 
			+ std::abs ( half_y_two * glm::dot ( t_one->local_y_axis, t_two->local_y_axis ) ) 
			+ std::abs ( half_z_two * glm::dot ( t_one->local_y_axis, t_two->local_z_axis ) ) );

	if ( one_y > manifold.penetration )
	{
		manifold.penetration = one_y;
		manifold.normal = t_one->local_y_axis;
	}

	if ( one_y > 0.0f ) return false;

	float one_z = std::abs ( glm::dot ( distance, t_one->local_z_axis ) ) - ( half_z_one 
			+ std::abs ( half_x_two * glm::dot ( t_one->local_z_axis, t_two->local_x_axis ) ) 
			+ std::abs ( half_y_two * glm::dot ( t_one->local_z_axis, t_two->local_y_axis ) ) 
			+ std::abs ( half_z_two * glm::dot ( t_one->local_z_axis, t_two->local_z_axis ) ) );

	if ( one_z > manifold.penetration )
	{
		manifold.penetration = one_z;
		manifold.normal = t_one->local_z_axis;
	}

	if ( one_z > 0.0f ) return false;


	//	Project onto local axes of collider two
	float two_x = std::abs ( glm::dot ( distance, t_two->local_x_axis ) ) - ( half_x_two 
			+ std::abs ( half_x_one * glm::dot ( t_one->local_x_axis, t_two->local_x_axis ) ) 
			+ std::abs ( half_y_one * glm::dot ( t_one->local_y_axis, t_two->local_x_axis ) ) 
			+ std::abs ( half_z_one * glm::dot ( t_one->local_z_axis, t_two->local_x_axis ) ) );

	if ( two_x > manifold.penetration )
	{
		manifold.penetration = two_x;
		manifold.normal = t_two->local_x_axis;
	}

	if ( two_x > 0.0f ) return false;

	float two_y = std::abs ( glm::dot ( distance, t_two->local_y_axis ) ) - ( half_y_two 
			+ std::abs ( half_x_one * glm::dot ( t_one->local_x_axis, t_two->local_y_axis ) ) 
			+ std::abs ( half_y_one * glm::dot ( t_one->local_y_axis, t_two->local_y_axis ) ) 
			+ std::abs ( half_z_one * glm::dot ( t_one->local_z_axis, t_two->local_y_axis ) ) );

	if ( two_y > manifold.penetration )
	{
		manifold.penetration = two_y;
		manifold.normal = t_two->local_y_axis;
	}

	if ( two_y > 0.0f ) return false;

	float two_z = std::abs ( glm::dot ( distance, t_two->local_z_axis ) ) - ( half_z_two 
			+ std::abs ( half_x_one * glm::dot ( t_one->local_x_axis, t_two->local_z_axis ) ) 
			+ std::abs ( half_y_one * glm::dot ( t_one->local_y_axis, t_two->local_z_axis ) ) 
			+ std::abs ( half_z_one * glm::dot ( t_one->local_z_axis, t_two->local_z_axis ) ) );

	if ( two_z > manifold.penetration )
	{
		manifold.penetration = two_z;
		manifold.normal = t_two->local_z_axis;
	}

	if ( two_z > 0.0f ) return false;

	return true;
}
