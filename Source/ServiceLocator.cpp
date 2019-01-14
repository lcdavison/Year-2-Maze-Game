#include "Includes/ServiceLocator.h"

//	Initialize all services to nullptrs
std::shared_ptr < ResourceManager > ServiceLocator::resource_manager = nullptr;
std::shared_ptr < EntityComponentSystem > ServiceLocator::ecs = nullptr;
std::shared_ptr < Window > ServiceLocator::window = nullptr;
std::shared_ptr < InputManager > ServiceLocator::input_manager = nullptr;
std::shared_ptr < Time > ServiceLocator::time = nullptr;

/*
	ProvideWindow

	Used to set the window service.
*/
void ServiceLocator::ProvideWindow ( std::shared_ptr < Window > win )
{
	window = win;
}

/*
	LocateWindow

	Used to return a pointer to the window service.
*/
std::shared_ptr < Window > ServiceLocator::LocateWindow (  )
{
	return window;
}

/*
	ProvideEntityComponentSystem

	Used to set the entity-component system service.
*/
void ServiceLocator::ProvideEntityComponentSystem ( std::shared_ptr < EntityComponentSystem > e_c_s )
{
	ecs = e_c_s;
}

/*
	LocateEntityComponentSystem

	Used to return a pointer to the entity-component system service.
*/
std::shared_ptr < EntityComponentSystem > ServiceLocator::LocateEntityComponentSystem (  )
{
	return ecs;
}

/*
	ProvideResourceManager

	Used to set the resource manager service.
*/
void ServiceLocator::ProvideResourceManager ( std::shared_ptr < ResourceManager > rm )
{
	resource_manager = rm;
}

/*
	LocateResourceManager

	Used to return a pointer to the resource manager service.
*/
std::shared_ptr < ResourceManager > ServiceLocator::LocateResourceManager (  )
{
	return resource_manager;
}

/*
	ProvideInputManager

	Used to set the input manager service.
*/
void ServiceLocator::ProvideInputManager ( std::shared_ptr < InputManager > im )
{
	input_manager = im;
}

/*
	LocateInputManager

	Used to return a pointer to the input manager service.
*/
std::shared_ptr < InputManager > ServiceLocator::LocateInputManager (  )
{
	return input_manager;
}

/*
	ProvideTime

	Used to set the time service.
*/
void ServiceLocator::ProvideTime ( std::shared_ptr < Time > ti )
{
	time = ti;
}

/*
	LocateTime

	Used to return a pointer to the time service.
*/
std::shared_ptr < Time > ServiceLocator::LocateTime (  )
{
	return time;
}
