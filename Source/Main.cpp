#include "Includes/Game.h"

#include <memory>

int main ( int argc, char** argv )
{
	std::shared_ptr < Game > game = std::make_shared < Game > (  );
	game->Start (  );

	return 0;
}
