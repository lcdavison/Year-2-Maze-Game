#include "Includes/Window.h"

Window::Window ( const char* title, const unsigned int& width, const unsigned int& height ) 
	: width ( width ), height ( height )
{
	this->title = new char [ strlen ( title ) + 1 ];
	strcpy ( this->title, title );
}

bool Window::CreateWindow (  )
{
	window = SDL_CreateWindow ( title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL );

	if ( window == nullptr )
	{
		std::cerr << "Failed to create window" << std::endl;
		return false;
	}

	SDL_GL_SetAttribute ( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

	SDL_GL_SetAttribute ( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
	SDL_GL_SetAttribute ( SDL_GL_CONTEXT_MINOR_VERSION, 6 );

	SDL_GL_SetAttribute ( SDL_GL_MULTISAMPLEBUFFERS, 1 );
	SDL_GL_SetAttribute ( SDL_GL_MULTISAMPLESAMPLES, 4 );
 
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
