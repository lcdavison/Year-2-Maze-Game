#include "Includes/Game.h"

// TODO: Comment and tidy code
// TODO: Command To Quit Game if escape is pressed?

Game::Game (  )
{
}

/*
   Start

   This function is used to setup the game, this involves loading models
   and setting up all the services.
*/
void Game::Start (  )
{
	//	Initialize SDL
	if ( SDL_Init ( SDL_INIT_VIDEO ) < 0 )
		std::cout << "Failed to initialize SDL2" << std::endl;

	//	Create services
	window = std::make_shared < Window > ( "3D Maze Game", 1280, 720 );
	window->CreateWindow (  );

	renderer = std::make_shared < Renderer > ( window );
	ecs = std::make_shared < EntityComponentSystem > (  );
	input = std::make_shared < InputManager > (  );
	time = std::make_shared < Time > (  );

	//	Send services to the service locator
	ServiceLocator::ProvideWindow ( window );
	ServiceLocator::ProvideEntityComponentSystem ( ecs );
	ServiceLocator::ProvideResourceManager ( std::make_shared < ResourceManager > (  ) );
	ServiceLocator::ProvideInputManager ( input );
	ServiceLocator::ProvideTime ( time );

	//	Load models into the resource manager
	ServiceLocator::LocateResourceManager (  )->LoadModel ( "resources/models/Plane.obj" );
	ServiceLocator::LocateResourceManager (  )->LoadModel ( "resources/models/Wall.obj" );

	//	Construct the level
	BuildLevel (  );

	//	Start the renderer
	renderer->Initialize (  );

	//	Set the state as running
	running = true;

	//	Begin the main game loop
	MainLoop (  );
}

/*
	Stop
	
	This function is used to stop the game.
*/
void Game::Stop (  )
{
}

/*
   MainLoop

   This function contains the main game loop that runs every single frame
*/
void Game::MainLoop (  )
{
	// Cache Delta Time, and convert to milliseconds
	const float delta_time = time->GetDeltaTime (  ) * 1000.0f;

	unsigned int last_time = SDL_GetTicks (  );
	unsigned int current_time = 0;	//	Current time
	float lag_counter = 0.0f;	//	Accumulates delta time to ensure consistent game speed across multiple devices

	SDL_Event event;

	while ( running )
	{
		current_time = SDL_GetTicks (  );
		unsigned int frame_time = current_time - last_time;	//	Calculate time taken to process the last frame
		last_time = current_time;

		lag_counter += frame_time;	//	Add frame time to lag_counter

		//	Handle SDL events 
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

		ProcessInput (  );	//	Process user input

		//	Update game when enough frame time has been accumulated
		while ( lag_counter >= delta_time )
		{
			Update ( delta_time * 0.001f );		//	Update the game with delta time in seconds

			lag_counter -= delta_time;	//	Remove delta_time from lag_counter
		}

		Render (  );	//	Render the game as fast possible
	}

	Stop (  );	//	TODO : Remove this function
}

/*
	Process Input

	Processes user input from the mouse to calculate camera rotations,
	and processes player commands and executes their actions.
*/
void Game::ProcessInput (  )
{
	//	Recapture the mouse if focus is lost
	SDL_CaptureMouse ( SDL_TRUE );

	//	Get mouse position
	glm::vec2 mouse_position = input->GetMousePosition (  );
	Transform* player_transform = ecs->GetTransform ( 0 );

	//	Set camera x rotation
	player_transform->rotation.x += -mouse_position.y * mouse_sensitivity.y;

	//	Clamp x rotation to 160 degrees vertically
	player_transform->rotation.x = glm::clamp ( player_transform->rotation.x, -80.0f, 80.0f );

	//	Set camera y rotation
	player_transform->rotation.y += mouse_position.x * mouse_sensitivity.x;

	//	Process player commands
	for ( Command command : ecs->GetPlayerController (  )->commands )
	{
		//	Determine command type
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

/*
	Update

	Updates the game world using the specified delta time.
*/
void Game::Update ( const float& delta )
{
	ecs->Update ( delta );
}

/*
	Render

	Renders the game world using the renderer.
*/
void Game::Render (  )
{
	renderer->Render (  );
}

/*
	BuildLevel

	Creates level entities, placing them in the world and setting thier models.
*/
void Game::BuildLevel (  )
{
	// Create Player
	const Entity* player = ecs->CreateEntity (  );
	ecs->SetPlayer ( player );

	//	Setup player controls
	ecs->AddPlayerCommand ( SDLK_a, &PlayerCommands::MoveLeft, KEYHELD );
	ecs->AddPlayerCommand ( SDLK_d, &PlayerCommands::MoveRight, KEYHELD );
	ecs->AddPlayerCommand ( SDLK_w, &PlayerCommands::MoveForward, KEYHELD );
	ecs->AddPlayerCommand ( SDLK_s, &PlayerCommands::MoveBackward, KEYHELD );
	ecs->AddPlayerCommand ( SDLK_r, &PlayerCommands::RetroToggle, KEYPRESS );

	//	Add box collider to the player
	BoxCollider* pbc = ecs->AddBoxCollider ( player, glm::vec3 ( -0.1f, -0.2f, -0.15f ), glm::vec3 ( 0.1f, 0.2f, 0.15f ) );

	// Build Level Layout
	// Floors
	for ( int x = 0; x < 2; x++ )
	{
		for ( int y = 0; y < 7; y++ )
		{
			const Entity* floor = ecs->CreateEntity (  );
			Transform* t = ecs->AddTransform ( floor );
			ecs->AddModel ( floor, "Plane.obj" );

			t->position = glm::vec3 ( x << 1, 0, y << 1 );
			t->rotation = glm::vec3 ( 90.0f, 0.0f, 0.0f );
			t->scale = glm::vec3 ( 1.0f, 1.0f, 1.0f );
		}
	}

	for ( int x = 0; x > -17; x-- )
	{
		const Entity* floor = ecs->CreateEntity (  );
		Transform* t = ecs->AddTransform ( floor );
		ecs->AddModel ( floor, "Plane.obj" );

		t->position = glm::vec3 ( x << 1, 0, 7 << 1 );
		t->rotation = glm::vec3 ( 90.0f, 0.0f, 0.0f );
		t->scale = glm::vec3 ( 1.0f, 1.0f, 1.0f );
	}

	for ( int x = 0; x < 15; x++ )
	{
		const Entity* floor = ecs->CreateEntity (  );
		Transform* t = ecs->AddTransform ( floor );
		ecs->AddModel ( floor, "Plane.obj" );

		t->position = glm::vec3 ( x << 1, 0, 7 << 1 );
		t->rotation = glm::vec3 ( 90.0f, 0.0f, 0.0f );
		t->scale = glm::vec3 ( 1.0f, 1.0f, 1.0f );
	}

	for ( int y = 6; y > 3; y-- )
	{
		const Entity* floor = ecs->CreateEntity (  );
		Transform* t = ecs->AddTransform ( floor );
		ecs->AddModel ( floor, "Plane.obj" );

		t->position = glm::vec3 ( -11 << 1, 0, y << 1 );
		t->rotation = glm::vec3 ( 90.0f, 0.0f, 0.0f );
		t->scale = glm::vec3 ( 1.0f, 1.0f, 1.0f );
	}



	for ( int y = 0; y < 6; y++ )
	{
		const Entity* floor = ecs->CreateEntity (  );
		Transform* t = ecs->AddTransform ( floor );
		ecs->AddModel ( floor, "Plane.obj" );

		t->position = glm::vec3 ( 14 << 1, 0, ( 7 + y ) << 1 );
		t->rotation = glm::vec3 ( 90.0f, 0.0f, 0.0f );
		t->scale = glm::vec3 ( 1.0f, 1.0f, 1.0f );
	}

	for ( int y = 0; y < 11; y++ )
	{
		const Entity* floor = ecs->CreateEntity (  );
		Transform* t = ecs->AddTransform ( floor );
		ecs->AddModel ( floor, "Plane.obj" );

		t->position = glm::vec3 ( -16 << 1, 0, ( 7 + y ) << 1 );
		t->rotation = glm::vec3 ( 90.0f, 0.0f, 0.0f );
		t->scale = glm::vec3 ( 1.0f, 1.0f, 1.0f );
	}

	for ( int x = 0; x < 3; x++ )
	{
		const Entity* floor = ecs->CreateEntity (  );
		Transform* t = ecs->AddTransform ( floor );
		ecs->AddModel ( floor, "Plane.obj" );

		t->position = glm::vec3 ( ( -16 - x ) << 1, 0, 14 << 1 );
		t->rotation = glm::vec3 ( 90.0f, 0.0f, 0.0f );
		t->scale = glm::vec3 ( 1.0f, 1.0f, 1.0f );
	}

	// Build Walls
	for ( int x = 0; x < 2; x++ )
	{
		const Entity* wall = ecs->CreateEntity (  );
		Transform* t = ecs->AddTransform ( wall );
		ecs->AddModel ( wall, "Wall.obj" );

		t->position = glm::vec3 ( x << 1, 1, -1 );
		t->rotation = glm::vec3 ( 0.0f, 0.0f, 0.0f );
		t->scale = glm::vec3 ( 1.0f, 1.0f, 1.0f );

		BoxCollider* collider = ecs->AddBoxCollider ( wall, ServiceLocator::LocateResourceManager (  )->GetModel ( "Wall.obj" ) );
	}

	for ( int y = 0; y < 7; y++ )
	{
		const Entity* wall = ecs->CreateEntity (  );
		Transform* t = ecs->AddTransform ( wall );
		ecs->AddModel ( wall, "Wall.obj" );

		t->position = glm::vec3 ( -1, 1, y << 1 );
		t->rotation = glm::vec3 ( 0.0f, 90.0f, 0.0f );
		t->scale = glm::vec3 ( 1.0f, 1.0f, 1.0f );

		BoxCollider* collider = ecs->AddBoxCollider ( wall, ServiceLocator::LocateResourceManager (  )->GetModel ( "Wall.obj" ) );
	}

	for ( int y = 0; y < 7; y++ )
	{
		const Entity* wall = ecs->CreateEntity (  );
		Transform* t = ecs->AddTransform ( wall );
		ecs->AddModel ( wall, "Wall.obj" );

		t->position = glm::vec3 ( 3, 1, y << 1 );
		t->rotation = glm::vec3 ( 0.0f, 90.0f, 0.0f );
		t->scale = glm::vec3 ( 1.0f, 1.0f, 1.0f );

		BoxCollider* collider = ecs->AddBoxCollider ( wall, ServiceLocator::LocateResourceManager (  )->GetModel ( "Wall.obj" ) );
	}
}
