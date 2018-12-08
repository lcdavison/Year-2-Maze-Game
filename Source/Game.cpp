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
	input = std::make_shared < InputManager > (  );

	ServiceLocator::ProvideWindow ( window );
	ServiceLocator::ProvideEntityComponentSystem ( ecs );
	ServiceLocator::ProvideResourceManager ( std::make_shared < ResourceManager > (  ) );
	ServiceLocator::ProvideInputManager ( input );

	ServiceLocator::LocateResourceManager (  )->LoadModel ( "resources/models/Plane.obj" );

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

		//	Handle Input Here
		while ( SDL_PollEvent ( &event ) )
		{
			switch ( event.type )
			{
				case SDL_QUIT:
					running = false;
					break;
				case SDL_KEYDOWN:
					input->SetKeyDown ( event.key.keysym.scancode );
					break;
				case SDL_KEYUP:
					input->SetKeyUp ( event.key.keysym.scancode );
					break;
			}
		}

		ProcessInput (  );

		while ( accumulator >= delta_time )
		{
			Update ( delta_time / 100 );

			accumulator -= delta_time;
		}

		Render (  );
	}
}

void Game::ProcessInput (  )
{
	for ( Command command : ecs->GetPlayerController (  )->commands )
	{
		switch ( command.type )
		{
			case KEYPRESS:
				if ( input->GetKeyDown ( command.key ) )
					command.Action (  );
				break;
			case KEYRELEASE:
				if ( input->GetKeyUp ( command.key ) )
					command.Action (  );
				break;
			case KEYHELD:
				if ( input->GetKeyHeld ( command.key ) )
					command.Action (  );
				break;
		}
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
	const Entity* test = ecs->CreateEntity (  );
	ecs->SetPlayer ( test );
	ecs->AddPlayerCommand ( SDLK_a, &PlayerCommands::MoveLeft, KEYHELD );
	ecs->AddPlayerCommand ( SDLK_d, &PlayerCommands::MoveRight, KEYHELD );
	ecs->AddPlayerCommand ( SDLK_w, &PlayerCommands::MoveForward, KEYHELD );
	ecs->AddPlayerCommand ( SDLK_s, &PlayerCommands::MoveBackward, KEYHELD );

	for ( int x = 0; x < 10; x++ )
	{
		for ( int y = 0; y < 10; y++ )
		{
			const Entity* floor = ecs->CreateEntity (  );
			Transform* t = ecs->AddTransform ( floor );
			ecs->AddModel ( floor, "Plane.obj" );

			t->position = glm::vec3 ( x * 2, 0, y * 2 );
			t->rotation = glm::vec3 ( 90.0f, 0.0f, 0.0f );
			t->scale = glm::vec3 ( 1.0f, 1.0f, 1.0f );			
		}
	}
}
