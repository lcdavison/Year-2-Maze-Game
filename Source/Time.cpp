#include "includes/Time.h"

Time::Time (  )
{
}

/*
	GetDeltaTime

	Returns the delta time for updating the game.
*/
const float& Time::GetDeltaTime (  )
{
	return delta_time;
}
