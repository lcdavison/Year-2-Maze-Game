#pragma once

#include "../ServiceLocator.h"

#include <iostream>

namespace PlayerCommands 
{
	namespace
	{
		std::shared_ptr < EntityComponentSystem > ecs = nullptr;
	};

	//  TODO: Cache Player
	static void MoveLeft (  )
	{
		if ( ecs == nullptr )
			ecs = ServiceLocator::LocateEntityComponentSystem (  );

		const Entity* player = ecs->GetEntity ( 0 );
		Transform* transform = ecs->GetTransform ( player );

		transform->position.x -= 0.1f;
		
		std::cout << "Move Left : " << transform->position.x << std::endl;
	}

	static void MoveRight (  )
	{
		if ( ecs == nullptr )
			ecs = ServiceLocator::LocateEntityComponentSystem (  );

		const Entity* player = ecs->GetEntity ( 0 );
		Transform* transform = ecs->GetTransform ( player );

		transform->position.x += 0.1f;
		
		std::cout << "Move Right" << std::endl;
	}

	static void MoveForward (  )
	{
		if ( ecs == nullptr )
			ecs = ServiceLocator::LocateEntityComponentSystem (  );

		const Entity* player = ecs->GetEntity ( 0 );
		Transform* transform = ecs->GetTransform ( player );

		transform->position.z -= 0.1f;
		
		std::cout << "Move Forward" << std::endl;
	}

	static void MoveBackward (  )
	{
		if ( ecs == nullptr )
			ecs = ServiceLocator::LocateEntityComponentSystem (  );

		const Entity* player = ecs->GetEntity ( 0 );
		Transform* transform = ecs->GetTransform ( player );

		transform->position.z += 0.1f;
		
		std::cout << "Move Backward" << std::endl;
	}
};
