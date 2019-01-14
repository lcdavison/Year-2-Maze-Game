#pragma once

#include <glew.h>

#include "Shader.h"
#include "ServiceLocator.h"

#include <vector>

/*
	The shader program class is used to handle the compilation of shaders,
	and the creation of the shader program executable that will run on the
	GPU.
*/
class ShaderProgram 
{
	public:
		ShaderProgram (  );		
	
		void CreateShader ( const char*, GLenum);
		void CompileShaders (  );
		void LinkProgram (  );

		void UseProgram (  );

		unsigned int GetProgramID (  );

	private:
		
		unsigned int program_id;

		std::vector < Shader > shaders;		//	The loaded shaders
};
