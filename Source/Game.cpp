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

	ServiceLocator::ProvideWindow ( window );
	ServiceLocator::ProvideEntityComponentSystem ( ecs );
	ServiceLocator::ProvideResourceManager ( std::make_shared < ResourceManager > (  ) );

	BuildLevel (  );

	renderer->Initialize (  );

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
}

void Game::Render (  )
{
	renderer->Render (  );
}

void Game::BuildLevel (  )
{
	for ( int i = 0; i < 10; i++ )
	{
		const Entity* floor = ecs->CreateEntity (  );
		Transform* t = ecs->AddTransform ( floor );
		Model* m = ecs->AddModel ( floor );

		t->position = glm::vec3 ( 0, 0, i * 2 );
		t->rotation = glm::vec3 ( 90.0f, 0.0f, 0.0f );
		t->scale = glm::vec3 ( 1.0f, 1.0f, 1.0f );

		ServiceLocator::LocateResourceManager (  )->LoadModel ( "resources/models/Plane.obj", m );
	}
}
