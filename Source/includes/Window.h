#pragma once

#include <SDL.h>

#include <iostream>
#include <cstring>

/*
	The window class is used to handle the window that displays the content of the game.
	This class is built on top of the SDL library, which is used to create the window and
	OpenGL context.
*/
class Window 
{
	public:

		Window ( const char*, const unsigned int&, const unsigned int& );
		~Window (  );

		bool CreateWindow (  );
		void ShowWindow (  );

		SDL_Window* GetWindowHandle (  );

	private:
		
		char* title = nullptr;		//	Window title
		unsigned int width, height;	//	Window width + height

		SDL_Window* window = nullptr;	//	SDL Window
};
