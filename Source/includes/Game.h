#pragma once

#include "Renderer.h"
#include "ServiceLocator.h"

#include "Scripting/PlayerCommands.h"

#include <iostream>
#include <memory>

class Game 
{
	public:
		
		Game (  );

		void Start (  );
		void Stop (  );

	private:

		void MainLoop (  );

		void BuildLevel (  );

		void ProcessInput (  );
		void Update ( const float& );
		void Render (  );

		bool running = false;

		glm::vec2 mouse_sensitivity = glm::vec2 ( 0.1f, 0.1f );

		std::shared_ptr < Window > window;
		std::shared_ptr < Renderer > renderer;
		std::shared_ptr < EntityComponentSystem > ecs;
		std::shared_ptr < InputManager > input;
		std::shared_ptr < Time > time;
};
