#include "Includes/Renderer.h"

//	TODO: Use callback to recalculate the projection matrix

Renderer::Renderer ( std::shared_ptr < Window > window ) : window ( window )
{
	if ( glewInit (  ) != GLEW_OK )
		std::cerr << "Failed To Initialize GLEW" << std::endl;

	glEnable ( GL_DEPTH_TEST );
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
		CreateModel ( model );
	}
}

void Renderer::Render (  )
{
	Clear (  );

	program->UseProgram (  );

	glm::mat4 projection = glm::perspective ( glm::radians ( 90.0f ), 1280.0f / 720.0f, 0.1f, 100.0f );

	glm::mat4 view = glm::mat4 ( 1 );
	view = glm::translate ( view, -1.0f * glm::vec3 ( 0, glm::sin ( SDL_GetTicks (  ) * 0.01f ), 5 ) );

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

void Renderer::CreateModel ( Model& model )
{
	for ( Mesh& mesh : model.meshes )	
	{
		glGenVertexArrays ( 1, &mesh.vertex_array );
		glBindVertexArray ( mesh.vertex_array );

		glGenBuffers ( 1, &mesh.vertex_buffer );
		glBindBuffer ( GL_ARRAY_BUFFER, mesh.vertex_buffer );
		glBufferData ( GL_ARRAY_BUFFER, mesh.vertices.size (  ) * sizeof ( Vertex ), &mesh.vertices [ 0 ], GL_STATIC_DRAW );

		glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, sizeof ( Vertex ), ( void* ) 0 );
		glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE, sizeof ( Vertex ), ( void* ) offsetof ( Vertex, normal ) );
		//glVertexAttribPointer ( 2, 2, GL_FLOAT, GL_FALSE, sizeof ( Vertex ), ( void* ) offsetof ( Vertex, texture_coordinates ) );

		glGenBuffers ( 1, &mesh.index_buffer );
		glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, mesh.index_buffer );
		glBufferData ( GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size (  ) * sizeof ( unsigned int ), &mesh.indices [ 0 ], GL_STATIC_DRAW );

		glBindBuffer ( GL_ARRAY_BUFFER, 0 );
		glBindVertexArray ( 0 );
	}
}

void Renderer::RenderModel ( Model& model )
{
	for ( Mesh mesh : model.meshes )
	{
		glBindVertexArray ( mesh.vertex_array );

		glEnableVertexAttribArray ( 0 );
		glEnableVertexAttribArray ( 1 );
		//glEnableVertexAttribArray ( 2 );
		
		//glPolygonMode ( GL_FRONT_AND_BACK, GL_POINT );
		glDrawElements ( GL_TRIANGLES, mesh.indices.size (  ), GL_UNSIGNED_INT, NULL );

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
	transformation = glm::rotate ( transformation, transform->rotation.z, glm::vec3 ( 0, 0, 1 ) );
	transformation = glm::rotate ( transformation, transform->rotation.y, glm::vec3 ( 0, 1, 0 ) );
	transformation = glm::rotate ( transformation, transform->rotation.x, glm::vec3 ( 1, 0, 0 ) );

	return transformation;
}
