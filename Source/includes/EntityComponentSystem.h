#pragma once

#include "Entity.h"

#include "Transform.h"
#include "Model.h"

#include <iostream>
#include <memory>
#include <vector>

class EntityComponentSystem 
{
	public:
	
		EntityComponentSystem (  );

		//	Return const pointers
		const Entity* CreateEntity ( std::string name = "Entity_" );
		const Entity* GetEntity ( const unsigned int& );

		Transform* AddTransform ( const Entity* );
		Transform* GetTransform ( const Entity* );

		Model* AddModel ( const Entity* );
		Model* GetModel ( const Entity* );

		void Update ( const float& );

		std::vector < Model >& GetModels (  );
		std::vector < Transform >& GetTransforms (  );

	private:

		unsigned int BinarySearch ( std::vector < Component >&, const Entity* );

		static unsigned int new_id;

		//	Stores Entities
		std::vector < Entity > entities;

		//	Stores Components
		std::vector < Transform > transforms;
		std::vector < Model > models; 
};
