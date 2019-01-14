#pragma once

#include <glew.h>
#include <glm/glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Entity.h"
#include "ModelData.h"

#include "Components/Transform.h"
#include "Components/PlayerController.h"
#include "Components/Model.h"
#include "Components/Physics.h"
#include "Components/BoxCollider.h"

#include <iostream>
#include <memory>
#include <vector>
#include <limits>

/*
	The collision manifold contains data that can be used to resolve a collision.
*/
struct CollisionManifold 
{
	float penetration;
	glm::vec3 normal;
};

/*
  EntityComponentSystem provides the ability to store entities and their components,
  while enabling players to retrieve and manipulate them. Each Component is stored 
  together with other components of the same type, to ensure data locality.
*/
class EntityComponentSystem 
{
	public:
	
		EntityComponentSystem (  );
		~EntityComponentSystem (  );

		const Entity* CreateEntity ( std::string name = "Entity_" );
		const Entity* GetEntity ( unsigned int );

		/*--TRANSFORMS--*/
		Transform* AddTransform ( const Entity* );
		Transform* GetTransform ( unsigned int );

		/*--MODELS--*/
		Model* AddModel ( const Entity*, const std::string& );
		Model* GetModel ( unsigned int );

		/*--PHYSICS--*/
		Physics* AddPhysics ( const Entity* );
		Physics* GetPhysics ( unsigned int );

		/*--BOX COLLIDERS--*/
		BoxCollider* AddBoxCollider ( const Entity*, const glm::vec3&, const glm::vec3& );
		BoxCollider* AddBoxCollider ( const Entity*, const ModelData&& );
		BoxCollider* GetBoxCollider ( unsigned int );

		/*--PLAYER--*/
		void SetPlayer ( const Entity* );
		void AddPlayerCommand ( const SDL_Keycode&, void ( *action ) (  ), const COMMAND_TYPE& );
		const PlayerController* GetPlayerController (  );

		void Update ( const float& );

		/*--GETTERS--*/
		std::vector < Model >& GetModels (  );
		std::vector < Transform >& GetTransforms (  );
		std::vector < BoxCollider >& GetBoxColliders (  );

	private:

		void Clear (  );

		bool CheckBoxCollision ( const BoxCollider&, const BoxCollider&, CollisionManifold& );

		//	Stores the next unique entity id
		static unsigned int new_id;

		//	Stores Entities
		std::vector < Entity > entities;

		//	Stores the player controller
		PlayerController player_controller;

		//	Stores Components
		std::vector < Transform > transforms;
		std::vector < Model > models;
		std::vector < Physics > physics;
		std::vector < BoxCollider > box_colliders;
};
