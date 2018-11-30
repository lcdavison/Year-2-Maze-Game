#pragma once

#include <glew.h>
#include <SDL.h>

#include "Window.h"
#include "ShaderProgram.h"

#include <memory>

class Renderer 
{
	public:
		
		Renderer ( std::shared_ptr < Window > );
		~Renderer (  );

		void Render (  );

	private:

		void Clear (  );

		std::shared_ptr < Window > window;
};
