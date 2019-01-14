#pragma once

#include "Renderer.h"
#include "ServiceLocator.h"

#include "Scripting/PlayerCommands.h"

#include <iostream>
#include <memory>

/*
  Game is the core class for the Maze Game, providing the game loop used for executing all of the games events.
  This includes updating the entities in the game, rendering the mesh data for the entities, and collecting input information
  to allow the user to interact with the game.
*/
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

		//	Stores the sensitivity that is applied to the collected mouse input
		glm::vec2 mouse_sensitivity = glm::vec2 ( 0.1f, 0.1f );

		//	Caches the pointers to game subsystems, for use in the game loop
		std::shared_ptr < Window > window;
		std::shared_ptr < Renderer > renderer;
		std::shared_ptr < EntityComponentSystem > ecs;
		std::shared_ptr < InputManager > input;
		std::shared_ptr < Time > time;
};
