#include "Includes/Renderer.h"

ShaderProgram* program = nullptr;
Model test;

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
	program = new ShaderProgram (  );
	program->CreateShader ( "resources/vertex.glsl", GL_VERTEX_SHADER );
	program->CreateShader ( "resources/fragment.glsl", GL_FRAGMENT_SHADER );
	program->CompileShaders (  );
	program->LinkProgram (  );

	ServiceLocator::LocateResourceManager (  )->LoadModel ( "resources/models/monkey.obj", test.meshes );

	CreateModel ( test );
}

void Renderer::Render (  )
{
	Clear (  );

	program->UseProgram (  );

	int x, y;
	SDL_GetMouseState ( &x, &y );

	glm::mat4 model = glm::mat4 ( 1.0 );
	model = glm::rotate ( model, glm::radians ( SDL_GetTicks (  ) * 0.01f ), glm::vec3 ( 0, 1, 0 ) );

	glm::mat4 projection = glm::perspective ( glm::radians ( 90.0f ), 1280.0f / 720.0f, 0.1f, 100.0f );
	glm::mat4 view = glm::lookAt ( glm::vec3 ( 0, 0, 2 ), glm::vec3 ( 0, 0, 0 ), glm::vec3 ( 0, 1, 0 ) );

	glm::mat4 mvp = projection * view * model;

	int mvp_loc = glGetUniformLocation ( program->GetProgramID (  ), "mvp" );
	glUniformMatrix4fv ( mvp_loc, 1, GL_FALSE, &mvp [ 0 ] [ 0 ] );

	int time_loc = glGetUniformLocation ( program->GetProgramID (  ), "time" );
	glUniform1f ( time_loc, ( float ) SDL_GetTicks (  ) );

	for ( Mesh mesh : test.meshes )
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
