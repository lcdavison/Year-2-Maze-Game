#include "Includes/Window.h"

Window::Window ( const char* title, const unsigned int& width, const unsigned int& height ) 
	: width ( width ), height ( height )
{
	//	Set window title
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

/*
	CreateWindow

	Used to create a new SDL window.
*/
bool Window::CreateWindow (  )
{
	//	Creates window
	window = SDL_CreateWindow ( title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL );

	//	Check if creation was successfull
	if ( window == nullptr )
	{
		std::cerr << "Failed to create window" << std::endl;
		return false;
	}

	//	Setup Mouse
	SDL_SetRelativeMouseMode ( SDL_TRUE );
	SDL_CaptureMouse ( SDL_TRUE );
	SDL_WarpMouseInWindow ( window, width >> 1, height >> 1 );

	//	Setup OpenGL context for the window
	SDL_GLContext context = SDL_GL_CreateContext ( window );

	//	Setup profile mask
	SDL_GL_SetAttribute ( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	
	//	Set OpenGL version
	SDL_GL_SetAttribute ( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
	SDL_GL_SetAttribute ( SDL_GL_CONTEXT_MINOR_VERSION, 5 );
	
	//	Set buffer sizes
	SDL_GL_SetAttribute ( SDL_GL_RED_SIZE, 8 );
	SDL_GL_SetAttribute ( SDL_GL_GREEN_SIZE, 8 );
	SDL_GL_SetAttribute ( SDL_GL_BLUE_SIZE, 8 );
	SDL_GL_SetAttribute ( SDL_GL_ALPHA_SIZE, 8 );
	SDL_GL_SetAttribute ( SDL_GL_DEPTH_SIZE, 16 );
	
	//	Enable multisampling buffers
	SDL_GL_SetAttribute ( SDL_GL_MULTISAMPLEBUFFERS, 1 );
	SDL_GL_SetAttribute ( SDL_GL_MULTISAMPLESAMPLES, 4 );

	//	Enable double buffering
	SDL_GL_SetAttribute ( SDL_GL_DOUBLEBUFFER, 1 );
 
	return true;
}

/*
	ShowWindow

	Used to show the window.
*/
void Window::ShowWindow (  )
{
	SDL_ShowWindow ( window );
}

/*
	GetWindowHandle

	Returns the SDL_Window handle for the window.
*/
SDL_Window* Window::GetWindowHandle (  )
{
	return window;
}
