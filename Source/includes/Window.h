#pragma once

#include <SDL.h>

#include <iostream>
#include <cstring>

class Window 
{
	public:

		Window ( const char*, const unsigned int&, const unsigned int& );
		~Window (  );

		bool CreateWindow (  );
		void ShowWindow (  );

		SDL_Window* GetWindowHandle (  );

	private:
		
		char* title = nullptr;
		unsigned int width, height;

		SDL_Window* window = nullptr;
};
