#include "Includes/Game.h"

#include <memory>

int main ( int argc, char** argv )
{
	//	Create a shared pointer to a Game object, and start the game
	std::shared_ptr < Game > game = std::make_shared < Game > (  );
	game->Start (  );

	return 0;
}
