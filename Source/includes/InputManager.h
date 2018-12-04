#pragma once

#include <iostream>
#include <SDL.h>

//	TODO: Mouse Input

class InputManager 
{
	public:

		InputManager (  );

		void SetKeyDown ( const SDL_Scancode& );
		void SetKeyUp ( const SDL_Scancode& );

		bool GetKeyDown ( const SDL_Keycode& );
		bool GetKeyUp ( const SDL_Keycode& );

	private:

		bool current_keyboard_state [ 285 ];
		bool previous_keyboard_state [ 285 ];
};
