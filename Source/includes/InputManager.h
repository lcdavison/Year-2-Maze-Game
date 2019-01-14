#pragma once

#include <iostream>
#include <SDL.h>

#include <glm/glm.hpp>

/*
  InputManager provides functionality to collect keyboard key events, 
  to determine whether a key has just been pressed or released.
  It can also be used to determine the mouses position.
*/
class InputManager 
{
	public:

		InputManager (  );

		void SetKeyDown ( const SDL_Scancode& );
		void SetKeyUp ( const SDL_Scancode& );

		bool GetKeyDown ( const SDL_Keycode& );
		bool GetKeyUp ( const SDL_Keycode& );
		bool GetKeyHeld ( const SDL_Keycode& );

		glm::vec2 GetMousePosition (  );

	private:

		//	Stores the previous mouse position
		glm::vec2 mouse_position;

		/*	Stores the current and previous keyboards to determine whether keys
			have been pressed or released. */
		bool current_keyboard_state [ 285 ];
		bool previous_keyboard_state [ 285 ];
};
