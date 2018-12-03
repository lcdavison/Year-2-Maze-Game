#pragma once

#include <glew.h>
#include <SDL.h>
#include <glm/glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "ServiceLocator.h"
#include "Window.h"
#include "ShaderProgram.h"
#include "Model.h"

#include <memory>

class Renderer 
{
	public:
		
		Renderer ( std::shared_ptr < Window > );
		~Renderer (  );

		void Initialize (  );

		void Render (  );

	private:

		void Clear (  );

		void CreateModel ( Model& );
		void RenderModel ( Model& );

		glm::mat4 CreateTransformation ( Transform* );

		std::shared_ptr < EntityComponentSystem > ecs;
		std::shared_ptr < Window > window;
		std::unique_ptr < ShaderProgram > program;
};
