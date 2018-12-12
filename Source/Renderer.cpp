#include "Includes/Renderer.h"

//	TODO: Use callback to recalculate the projection matrix
//	TODO: Create more effective Uniform management
//	TODO: Store the view matrix to enable collection of directions for FPS Camera

Renderer::Renderer ( std::shared_ptr < Window > window ) : window ( window )
{
	if ( glewInit (  ) != GLEW_OK )
		std::cerr << "Failed To Initialize GLEW" << std::endl;

	glEnable ( GL_DEPTH_TEST );
	
	glEnable ( GL_CULL_FACE );
	glCullFace ( GL_BACK );
	
	glClearColor ( 0.0, 0.0, 0.2, 1.0 );
}

Renderer::~Renderer (  )
{
}

void Renderer::Initialize (  )
{
	ecs = ServiceLocator::LocateEntityComponentSystem (  );

	program = std::make_unique < ShaderProgram > (  );
	program->CreateShader ( "resources/vertex.glsl", GL_VERTEX_SHADER );
	program->CreateShader ( "resources/fragment.glsl", GL_FRAGMENT_SHADER );
	program->CompileShaders (  );
	program->LinkProgram (  );

	for ( Model& model : ecs->GetModels (  ) )
	{
		SetupModel ( model );
	}

	projection = glm::perspective ( glm::radians ( 60.0f ), 1280.0f / 720.0f, 0.1f, 100.0f );
	player = ecs->GetEntity ( 0 );
}

void Renderer::Render (  )
{
	Clear (  );

	program->UseProgram (  );

	Transform* player_transform = ecs->GetTransform ( player );
	glm::mat4 view = CreateView ( player_transform );

	int proj_loc = glGetUniformLocation ( program->GetProgramID (  ), "projection" );
	glUniformMatrix4fv ( proj_loc, 1, GL_FALSE, &projection [ 0 ] [ 0 ] );

	int view_loc = glGetUniformLocation ( program->GetProgramID (  ), "view" );
	glUniformMatrix4fv ( view_loc, 1, GL_FALSE, &view [ 0 ] [ 0 ] );

	int model_loc = glGetUniformLocation ( program->GetProgramID (  ), "model" );
	
	for ( Model model : ecs->GetModels (  ) )	
	{
		const Entity* entity = ecs->GetEntity ( model.entity );

		Transform* entity_transform = ecs->GetTransform ( entity );

		glm::mat4 m = CreateTransformation ( entity_transform );

		glUniformMatrix4fv ( model_loc, 1, GL_FALSE, &m [ 0 ] [ 0 ] );

		RenderModel ( model );
	}

	SDL_GL_SwapWindow ( window->GetWindowHandle (  ) );
}

void Renderer::Clear (  )
{
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void Renderer::SetupModel ( Model& model )
{
	ServiceLocator::LocateResourceManager (  )->GetModel ( model.filename, &model.vertex_arrays, &model.num_vertices );
}

void Renderer::RenderModel ( Model& model )
{
	for ( int i = 0; i < model.vertex_arrays.size (  ); i++ )
	{
		glBindVertexArray ( model.vertex_arrays [ i ] );

		glEnableVertexAttribArray ( 0 );
		glEnableVertexAttribArray ( 1 );
		//glEnableVertexAttribArray ( 2 );
		
		//glPolygonMode ( GL_FRONT_AND_BACK, GL_POINT );
		glDrawElements ( GL_TRIANGLES, model.num_vertices [ i ], GL_UNSIGNED_INT, NULL );

		//glDisableVertexAttribArray ( 2 );
		glDisableVertexAttribArray ( 1 );
		glDisableVertexAttribArray ( 0 );

		glBindVertexArray ( 0 );
	}
}

glm::mat4 Renderer::CreateTransformation ( Transform* transform )
{
	glm::mat4 transformation = glm::mat4 ( 1 );

	transformation = glm::scale ( transformation, transform->scale );	
	transformation = glm::translate ( transformation, transform->position );
	transformation = glm::rotate ( transformation, glm::radians ( transform->rotation.z ), glm::vec3 ( 0, 0, 1 ) );
	transformation = glm::rotate ( transformation, glm::radians ( transform->rotation.y ), glm::vec3 ( 0, 1, 0 ) );
	transformation = glm::rotate ( transformation, glm::radians ( transform->rotation.x ), glm::vec3 ( 1, 0, 0 ) );

	transform->forward = glm::vec3 ( transformation [ 2 ][ 0 ], transformation [ 2 ][ 1 ], transformation [ 2 ][ 2 ] );

	return transformation;
}

glm::mat4 Renderer::CreateView ( Transform* transform )
{
	// TODO: Create Calculation for forward vector, to avoid matrix multiplication
	glm::mat4 transformation = glm::mat4 ( 1 );

	transformation = glm::translate ( transformation, transform->position );
	transformation = glm::rotate ( transformation, glm::radians ( transform->rotation.y ), glm::vec3 ( 0, 1, 0 ) );

	// Extracts forward move vector
	transform->forward = glm::vec3 ( transformation [ 2 ][ 0 ], transformation [ 2 ][ 1 ], transformation [ 2 ][ 2 ] );

	// Rotates for look matrix
	transformation = glm::rotate ( transformation, glm::radians ( transform->rotation.x ), glm::vec3 ( 1, 0, 0 ) );

	glm::vec3 forward = glm::vec3 ( transformation [ 2 ][ 0 ], transformation [ 2 ][ 1 ], transformation [ 2 ][ 2 ] );

	// Look in direction
	glm::mat4 view = glm::lookAt ( transform->position, transform->position + forward, glm::vec3 ( 0, 1, 0 ) );
	return view;
}
