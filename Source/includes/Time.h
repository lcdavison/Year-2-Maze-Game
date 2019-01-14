#pragma once

/*
	The time class is used to control the delta_time value used when integrating physics in the game.
*/
class Time
{
	public:
	
		Time (  );

		const float& GetDeltaTime (  );

	private:

		const float delta_time = 1.0f / 60.0f;	//	Delta time for 60fps = 0.0166666...
};
