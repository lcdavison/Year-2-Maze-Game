#include "Includes/InputManager.h"

InputManager::InputManager (  )
{
	//	Initialize the keyboard state to all keys up
	for ( int i = 0; i <= 284; i++ )
	{
		current_keyboard_state [ i ] = false;
		previous_keyboard_state [ i ] = false;
	}
}

/*
	SetKeyDown
	
	Sets the key to be down if it is currently up.
*/
void InputManager::SetKeyDown ( const SDL_Scancode& scancode )
{
	if ( current_keyboard_state [ scancode ] == false )
	{
		previous_keyboard_state [ scancode ] = current_keyboard_state [ scancode ];
		current_keyboard_state [ scancode ] = true;
	}
}

/*
	SetKeyUp

	Sets the key to be up if is is currently pressed down.
*/
void InputManager::SetKeyUp ( const SDL_Scancode& scancode )
{
	if ( current_keyboard_state [ scancode ] == true )
	{
		previous_keyboard_state [ scancode ] = current_keyboard_state [ scancode ];
		current_keyboard_state [ scancode ] = false;
	}
}

/*
	GetKeyDown

	Used to determine whether a key has been pressed down.
*/
bool InputManager::GetKeyDown ( const SDL_Keycode& key )
{	
	//	Convert to scancode
	SDL_Scancode scancode = SDL_GetScancodeFromKey ( key );

	//	Check key state
	if ( current_keyboard_state [ scancode ] == true && previous_keyboard_state [ scancode ] == false )
	{
		//	Update key state
		previous_keyboard_state [ scancode ] = current_keyboard_state [ scancode ];

		return true;
	}

	return false;
}

/*
	GetKeyUp

	Used to determine whether a key has been released.
*/
bool InputManager::GetKeyUp ( const SDL_Keycode& key )
{
	//	Convert to scancode
	SDL_Scancode scancode = SDL_GetScancodeFromKey ( key );

	//	Check key state
	if ( current_keyboard_state [ scancode ] == false && previous_keyboard_state [ scancode ] == true )
	{
		//	Update key state
		previous_keyboard_state [ scancode ] = current_keyboard_state [ scancode ];

		return true;
	}

	return false;
}

/*
	GetKeyHeld

	Used to determine if a key is currently held down.
*/
bool InputManager::GetKeyHeld ( const SDL_Keycode& key )
{
	//	Convert to scancode
	SDL_Scancode scancode = SDL_GetScancodeFromKey ( key );

	//	Return current state
	return current_keyboard_state [ scancode ];
}

/*
	GetMousePosition

	Returns the mouse position.
*/
glm::vec2 InputManager::GetMousePosition (  )
{
	//	Get current mouse state
	int x, y;
	SDL_GetMouseState ( &x, &y );

	//	Store position in a vector
	glm::vec2 new_mouse_position = glm::vec2 ( x, y );

	//	Calculate movement distance
	glm::vec2 result = mouse_position - new_mouse_position;

	//	Update mouse position
	mouse_position = new_mouse_position;
	
	return result;
}
