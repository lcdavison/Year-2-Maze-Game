#include "Includes/Window.h"

Window::Window ( const char* title, const unsigned int& width, const unsigned int& height ) 
	: width ( width ), height ( height )
{
	this->title = new char [ strlen ( title ) + 1 ];
	strcpy ( this->title, title );
}

Window::~Window (  )
{
	if ( window != nullptr )
		SDL_DestroyWindow ( window );

	if ( title != nullptr )
		delete title;
}

bool Window::CreateWindow (  )
{
	window = SDL_CreateWindow ( title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL );

	if ( window == nullptr )
	{
		std::cerr << "Failed to create window" << std::endl;
		return false;
	}

	SDL_GLContext context = SDL_GL_CreateContext ( window );

	SDL_GL_SetAttribute ( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	
	SDL_GL_SetAttribute ( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
	SDL_GL_SetAttribute ( SDL_GL_CONTEXT_MINOR_VERSION, 5 );
	
	SDL_GL_SetAttribute ( SDL_GL_RED_SIZE, 8 );
	SDL_GL_SetAttribute ( SDL_GL_GREEN_SIZE, 8 );
	SDL_GL_SetAttribute ( SDL_GL_BLUE_SIZE, 8 );
	SDL_GL_SetAttribute ( SDL_GL_ALPHA_SIZE, 8 );
	SDL_GL_SetAttribute ( SDL_GL_DEPTH_SIZE, 16 );
	
	SDL_GL_SetAttribute ( SDL_GL_MULTISAMPLEBUFFERS, 1 );
	SDL_GL_SetAttribute ( SDL_GL_MULTISAMPLESAMPLES, 4 );

	SDL_GL_SetAttribute ( SDL_GL_DOUBLEBUFFER, 1 );
 
	return true;
}

void Window::ShowWindow (  )
{
	SDL_ShowWindow ( window );
}

SDL_Window* Window::GetWindowHandle (  )
{
	return window;
}
