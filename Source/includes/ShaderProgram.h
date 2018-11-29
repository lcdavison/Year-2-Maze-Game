#pragma once

#include <glew.h>

#include "Shader.h"

#include <vector>

class ShaderProgram 
{
	public:
		ShaderProgram (  );		
	
		void CreateShader ( const char*, GLenum);
		void CompileShaders (  );
		void LinkProgram (  );

		void UseProgram (  );

	private:
		
		unsigned int program_id;

		std::vector < Shader > shaders;
};
