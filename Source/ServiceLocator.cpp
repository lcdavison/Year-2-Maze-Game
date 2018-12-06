#include "Includes/ServiceLocator.h"

std::shared_ptr < ResourceManager > ServiceLocator::resource_manager = nullptr;
std::shared_ptr < EntityComponentSystem > ServiceLocator::ecs = nullptr;
std::shared_ptr < Window > ServiceLocator::window = nullptr;
std::shared_ptr < InputManager > ServiceLocator::input_manager = nullptr;

void ServiceLocator::ProvideWindow ( std::shared_ptr < Window > win )
{
	window = win;
}

std::shared_ptr < Window > ServiceLocator::LocateWindow (  )
{
	return window;
}

void ServiceLocator::ProvideEntityComponentSystem ( std::shared_ptr < EntityComponentSystem > e_c_s )
{
	ecs = e_c_s;
}

std::shared_ptr < EntityComponentSystem > ServiceLocator::LocateEntityComponentSystem (  )
{
	return ecs;
}

void ServiceLocator::ProvideResourceManager ( std::shared_ptr < ResourceManager > rm )
{
	resource_manager = rm;
}

std::shared_ptr < ResourceManager > ServiceLocator::LocateResourceManager (  )
{
	return resource_manager;
}

void ServiceLocator::ProvideInputManager ( std::shared_ptr < InputManager > im )
{
	input_manager = im;
}

std::shared_ptr < InputManager > ServiceLocator::LocateInputManager (  )
{
	return input_manager;
}
