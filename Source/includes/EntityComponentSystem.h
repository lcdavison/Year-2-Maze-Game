#pragma once

#include "Entity.h"

#include <iostream>
#include <memory>
#include <vector>

class EntityComponentSystem 
{

	public:
	
		EntityComponentSystem (  );

		void CreateEntity ( std::string name = "Entity_" );
		void Print (  );

		void Update ( const float& );

	private:

		static unsigned int new_id;

		std::vector < Entity > entities;
};
