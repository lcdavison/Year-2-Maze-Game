#include "Includes/Renderer.h"

unsigned int vao, vbo;
ShaderProgram* program = nullptr;

Renderer::Renderer ( std::shared_ptr < Window > window ) : window ( window )
{
	SDL_GLContext context = SDL_GL_CreateContext ( window->GetWindowHandle (  ) );

	if ( glewInit (  ) != GLEW_OK )
		std::cerr << "Failed To Initialize GLEW" << std::endl;

	program = new ShaderProgram (  );
	program->CreateShader ( "resources/vertex.glsl", GL_VERTEX_SHADER );
	program->CreateShader ( "resources/fragment.glsl", GL_FRAGMENT_SHADER );
	program->CompileShaders (  );
	program->LinkProgram (  );

	glClearColor ( 0.0, 0.0, 0.2, 1.0 );

	float vertices [  ] = 
	{
		-0.5f, -0.5f,
		0.0f, 0.5f,
		0.5f, -0.5f
	};

	glGenVertexArrays ( 1, &vao );
	glBindVertexArray ( vao );

	glGenBuffers ( 1, &vbo );
	glBindBuffer ( GL_ARRAY_BUFFER, vbo );
	glBufferData ( GL_ARRAY_BUFFER, sizeof ( vertices ), &vertices [ 0 ], GL_STATIC_DRAW );
	glVertexAttribPointer ( 0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof ( float ), ( void* ) 0 );
	
	glBindBuffer ( GL_ARRAY_BUFFER, 0 );
	glBindVertexArray ( 0 );
}

Renderer::~Renderer (  )
{
}

void Renderer::Render (  )
{
	Clear (  );

	program->UseProgram (  );

	glBindVertexArray ( vao );

	glEnableVertexAttribArray ( 0 );

	glDrawArrays ( GL_TRIANGLES, 0, 3 );

	glDisableVertexAttribArray ( 0 );

	glBindVertexArray ( 0 );

	SDL_GL_SwapWindow ( window->GetWindowHandle (  ) );
}

void Renderer::Clear (  )
{
	glClear ( GL_COLOR_BUFFER_BIT );
}
