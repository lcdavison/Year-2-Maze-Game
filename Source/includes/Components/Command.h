#pragma once

#include <SDL.h>

enum COMMAND_TYPE { KEYPRESS, KEYRELEASE, KEYHELD };	//	Used to define the input a command looks for

/*
	The command data structure contains the data required to execute events 
	upon the user interacting with a key on the keyboard. These commands are
	stored in a controller, such as the player controller.
*/
struct Command 
{
	SDL_Keycode key;	// Key being watched
	void ( *Action ) (  );	// Function pointer for the command event
	COMMAND_TYPE type;	// Type of key event
};
