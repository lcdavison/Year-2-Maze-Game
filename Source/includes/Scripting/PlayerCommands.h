#pragma once

#include "../ServiceLocator.h"

#include <iostream>

namespace PlayerCommands 
{
	namespace
	{
		std::shared_ptr < EntityComponentSystem > ecs = nullptr;
		std::shared_ptr < Time > time = nullptr;

		static void GetECS (  )
		{
			if ( ecs == nullptr )
				ecs = ServiceLocator::LocateEntityComponentSystem (  );	
		}

		static void GetTime (  )
		{
			if ( time == nullptr )
				time = ServiceLocator::LocateTime (  );
		}
	}; 

	//  TODO: Cache Player
	static void MoveLeft (  )
	{
	        GetECS (  );
		GetTime (  );
		
		const Entity* player = ecs->GetEntity ( 0 );
		Transform* transform = ecs->GetTransform ( player );

		glm::vec3 right = glm::cross ( transform->forward, glm::vec3 ( 0, 1, 0 ) );

		transform->position -= 2.0f * right * ( time->GetDeltaTime (  ) * 0.001f );
	}

	static void MoveRight (  )
	{
		GetECS (  );
		GetTime (  );

		const Entity* player = ecs->GetEntity ( 0 );
		Transform* transform = ecs->GetTransform ( player );
		
		glm::vec3 right = glm::cross ( transform->forward, glm::vec3 ( 0, 1, 0 ) );

		transform->position += 2.0f * right * ( time->GetDeltaTime (  ) * 0.001f );
	}

	static void MoveForward (  )
	{
		GetECS (  );
		GetTime (  );

		const Entity* player = ecs->GetEntity ( 0 );
		Transform* transform = ecs->GetTransform ( player );

		transform->position += 2.0f * transform->forward * ( time->GetDeltaTime (  ) * 0.001f );
	}

	static void MoveBackward (  )
	{
		GetECS (  );
		GetTime (  );

		const Entity* player = ecs->GetEntity ( 0 );
		Transform* transform = ecs->GetTransform ( player );

		transform->position += 2.0f * -transform->forward * ( time->GetDeltaTime (  ) * 0.001f );
	}
};
