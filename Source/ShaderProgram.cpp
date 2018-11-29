#include "Includes/ShaderProgram.h"

ShaderProgram::ShaderProgram (  )
{
	program_id = glCreateProgram (  );	
}

void ShaderProgram::CreateShader ( const char* path, GLenum type )
{
	Shader new_shader;
	new_shader.type = type;
	new_shader.path = path;

	shaders.push_back ( new_shader );
}

void ShaderProgram::CompileShaders (  )
{
	for ( Shader shader : shaders )
	{
		shader.id = glCreateShader ( shader.type );

		// 	glShaderSource ( shader.id, 1, , NULL );
	}
}

void ShaderProgram::LinkProgram (  )
{

}

void ShaderProgram::UseProgram (  )
{
	glUseProgram ( program_id );
}
