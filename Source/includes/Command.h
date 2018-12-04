#pragma once

#include <SDL.h>

struct Command 
{
	SDL_Keycode key;
	void ( *OnKeyDown ) (  );
	void ( *OnKeyUp ) (  );
};
