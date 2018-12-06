#pragma once

#include "Entity.h"

#include "Components/Transform.h"
#include "Components/PlayerController.h"
#include "Components/Model.h"

#include <iostream>
#include <memory>
#include <vector>

class EntityComponentSystem 
{
	public:
	
		EntityComponentSystem (  );

		const Entity* CreateEntity ( std::string name = "Entity_" );
		const Entity* GetEntity ( const unsigned int& );

		Transform* AddTransform ( const Entity* );
		Transform* GetTransform ( const Entity* );

		Model* AddModel ( const Entity* );
		Model* GetModel ( const Entity* );

		void SetPlayer ( const Entity* );
		void AddPlayerCommand ( const SDL_Keycode&, void ( *action ) (  ), const COMMAND_TYPE& );
		const PlayerController* GetPlayerController (  );

		void Update ( const float& );

		std::vector < Model >& GetModels (  );
		std::vector < Transform >& GetTransforms (  );

	private:

		void Clear (  );
		void ClearModels (  );

		static unsigned int new_id;

		//	Stores Entities
		std::vector < Entity > entities;

		//	Stores Components
		PlayerController player_controller;
		std::vector < Transform > transforms;
		std::vector < Model > models; 
};
