#include "Includes/Renderer.h"

unsigned int vao, vbo;
Renderer::Renderer ( std::shared_ptr < Window > window ) : window ( window )
{
	SDL_GLContext context = SDL_GL_CreateContext ( window->GetWindowHandle (  ) );

	if ( glewInit (  ) != GLEW_OK )
		std::cerr << "Failed To Initialize GLEW" << std::endl;

	glClearColor ( 0.0, 0.0, 0.2, 1.0 );
}

Renderer::~Renderer (  )
{
}

void Renderer::Render (  )
{
	Clear (  );

	SDL_GL_SwapWindow ( window->GetWindowHandle (  ) );
}

void Renderer::Clear (  )
{
	glClear ( GL_COLOR_BUFFER_BIT );
}
