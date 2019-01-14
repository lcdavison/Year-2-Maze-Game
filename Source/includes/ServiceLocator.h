#pragma once

#include "Window.h"
#include "EntityComponentSystem.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Time.h"

#include <memory>

/*
	This is an implementation of the service locator pattern, which makes it much easier 
	to handle communication between services in the game engine.
*/
class ServiceLocator 
{
	public:
		
		static void ProvideWindow ( std::shared_ptr < Window > );
		static std::shared_ptr < Window > LocateWindow (  );

		static void ProvideEntityComponentSystem ( std::shared_ptr < EntityComponentSystem > );
		static std::shared_ptr < EntityComponentSystem > LocateEntityComponentSystem (  );		
 
		static void ProvideResourceManager ( std::shared_ptr < ResourceManager > );
		static std::shared_ptr < ResourceManager > LocateResourceManager (  );

		static void ProvideInputManager ( std::shared_ptr < InputManager > );
		static std::shared_ptr < InputManager > LocateInputManager (  );

		static void ProvideTime ( std::shared_ptr < Time > );
		static std::shared_ptr < Time > LocateTime (  );

	private:

		//	Pointers to services
		static std::shared_ptr < Window > window;
		static std::shared_ptr < EntityComponentSystem > ecs;
		static std::shared_ptr < ResourceManager > resource_manager;
		static std::shared_ptr < InputManager > input_manager;
		static std::shared_ptr < Time > time;
};
