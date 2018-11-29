#include "Includes/Game.h"

Game::Game (  )
{

}

void Game::Start (  )
{
	if ( SDL_Init ( SDL_INIT_VIDEO ) < 0 )
		std::cout << "Failed to initialize SDL2" << std::endl;

	window = std::make_shared < Window > ( "3D Maze Game", 1280, 720 );
	window->CreateWindow (  );

	renderer = std::make_shared < Renderer > ( window );
	ecs = std::make_shared < EntityComponentSystem > (  );

	running = true;

	MainLoop (  );
}

void Game::Stop (  )
{

}

void Game::MainLoop (  )
{
	unsigned int last_time = SDL_GetTicks (  );
	unsigned int current_time = 0;
	float accumulator = 0.0f;

	SDL_Event event;

	while ( running )
	{
		current_time = SDL_GetTicks (  );
		unsigned int frame_time = current_time - last_time;
		last_time = current_time;

		accumulator += frame_time;

		while ( SDL_PollEvent ( &event ) )
		{
			switch ( event.type )
			{
				case SDL_QUIT:
					running = false;
					break;
			}
		}

		while ( accumulator >= delta_time )
		{
			Update ( delta_time / 100 );

			accumulator -= delta_time;
		}

		Render (  );
	}
}

void Game::Update ( const float& delta )
{
	ecs->Update ( delta );
	std::cout << "UPDATE : " << delta << std::endl;
}

void Game::Render (  )
{
	renderer->Render (  );
}
