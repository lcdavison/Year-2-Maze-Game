#pragma once

#include "../ServiceLocator.h"

#include <iostream>

/*
	This namespace contains all the functions used by the player commands,
	this commands are used to control the player camera in the game world.
*/
namespace PlayerCommands 
{
	namespace
	{
		std::shared_ptr < EntityComponentSystem > ecs = nullptr;
		std::shared_ptr < Time > time = nullptr;

		Transform* transform = nullptr;
		Physics* physics = nullptr;

		const float delta_time = 1.0f / 60.0f;

		static void GetECS (  )
		{
			if ( ecs == nullptr )
				ecs = ServiceLocator::LocateEntityComponentSystem (  );	
		}

		static void GetTransform ( const Entity* player )
		{
			if ( transform == nullptr )
				transform = ecs->GetTransform ( player->id );
		}

		static void GetPhysics ( const Entity* player )
		{
			if ( physics == nullptr )
				physics = ecs->GetPhysics ( player->id );
		}

		static void Get (  )
		{
			GetECS (  );
		
			const Entity* player = ecs->GetEntity ( 0 );
			GetTransform ( player );
			GetPhysics ( player );
		}
	}; 

	static void MoveLeft (  )
	{
		Get (  );

		glm::vec3 right = glm::cross ( transform->forward, glm::vec3 ( 0, 1, 0 ) );

		physics->velocity -= ( 60.0f * glm::normalize ( right ) ) * delta_time;
	}

	static void MoveRight (  )
	{
		Get (  );
		
		glm::vec3 right = glm::cross ( transform->forward, glm::vec3 ( 0, 1, 0 ) );

		physics->velocity += ( 60.0f * glm::normalize ( right ) ) * delta_time;
	}

	static void MoveForward (  )
	{
		Get (  );

		physics->velocity += ( 60.0f * glm::normalize ( transform->forward ) ) * delta_time;
	}

	static void MoveBackward (  )
	{
		Get (  );

		physics->velocity -= ( 60.0f * glm::normalize ( transform->forward ) ) * delta_time;
	}

	static void RetroToggle (  )
	{
		Renderer::Retro_Mode = ( Renderer::Retro_Mode ) ? false : true;
	}
};
