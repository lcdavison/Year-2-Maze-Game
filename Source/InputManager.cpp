#include "Includes/InputManager.h"

InputManager::InputManager (  )
{
	for ( int i = 0; i <= 284; i++ )
	{
		current_keyboard_state [ i ] = false;
		previous_keyboard_state [ i ] = false;
	}

}

void InputManager::SetKeyDown ( const SDL_Scancode& scancode )
{
	if ( current_keyboard_state [ scancode ] == false )
	{
		previous_keyboard_state [ scancode ] = current_keyboard_state [ scancode ];
		current_keyboard_state [ scancode ] = true;
	}
}

void InputManager::SetKeyUp ( const SDL_Scancode& scancode )
{
	if ( current_keyboard_state [ scancode ] == true )
	{
		previous_keyboard_state [ scancode ] = current_keyboard_state [ scancode ];
		current_keyboard_state [ scancode ] = false;
	}
}

bool InputManager::GetKeyDown ( const SDL_Keycode& key )
{	
	SDL_Scancode scancode = SDL_GetScancodeFromKey ( key );

	if ( current_keyboard_state [ scancode ] == true && previous_keyboard_state [ scancode ] == false )
	{
		previous_keyboard_state [ scancode ] = current_keyboard_state [ scancode ];

		return true;
	}

	return false;
}

bool InputManager::GetKeyUp ( const SDL_Keycode& key )
{
	SDL_Scancode scancode = SDL_GetScancodeFromKey ( key );

	if ( current_keyboard_state [ scancode ] == false && previous_keyboard_state [ scancode ] == true )
	{
		previous_keyboard_state [ scancode ] = current_keyboard_state [ scancode ];

		return true;
	}

	return false;
}
