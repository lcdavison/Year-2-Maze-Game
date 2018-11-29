#pragma once

#include "Window.h"
#include "EntityComponentSystem.h"
#include "ResourceManager.h"

#include <memory>

class ServiceLocator 
{
	public:
		
		static void ProvideWindow ( std::shared_ptr < Window > );
		static std::shared_ptr < Window > LocateWindow (  );

		static void ProvideEntityComponentSystem ( std::shared_ptr < EntityComponentSystem > );
		static std::shared_ptr < EntityComponentSystem > LocateEntityComponentSystem (  );		
 
		static void ProvideResourceManager ( std::shared_ptr < ResourceManager > );
		static std::shared_ptr < ResourceManager > LocateResourceManager (  );

	private:

		static std::shared_ptr < Window > window;
		static std::shared_ptr < EntityComponentSystem > ecs;
		static std::shared_ptr < ResourceManager > resource_manager;
};
