#pragma once

#include <SDL.h>

enum COMMAND_TYPE { KEYPRESS, KEYRELEASE, KEYHELD };

struct Command 
{
	SDL_Keycode key;
	void ( *Action ) (  );
	COMMAND_TYPE type;
};
