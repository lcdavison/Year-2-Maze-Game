#pragma once

#include <glew.h>
#include <SDL.h>
#include <glm/glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "ServiceLocator.h"
#include "Window.h"
#include "ShaderProgram.h"

#include <memory>

/*
  Renderer provides functionality to draw elements to the screen, using the OpenGL API
*/
class Renderer 
{
	public:
		
		Renderer ( std::shared_ptr < Window > );
		~Renderer (  );

		void Initialize (  );

		void Render (  );

		static bool Retro_Mode;

	private:

		//	OpenGL buffer IDs for rendering colliders
		unsigned int collider_vao, collider_vbo, collider_ebo;

		void Clear (  );

		void RenderModel ( Model& );

		void SetupColliderData (  );
		void RenderBoxColliders (  );

		glm::mat4 CreateTransformation ( Transform* );
		glm::mat4 CreateView ( Transform* );

		glm::mat4 projection;	//	Cahced projection matrix

		const Entity* player;	//	Player entity

		//	Cached service pointers
		std::shared_ptr < EntityComponentSystem > ecs;
		std::shared_ptr < Window > window;
		std::unique_ptr < ShaderProgram > program;
};
