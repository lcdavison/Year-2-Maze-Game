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

		std::string shader_code;
		ServiceLocator::LocateResourceManager (  )->LoadShader ( shader.path, shader_code );

		const char* sc = shader_code.c_str (  );

		glShaderSource ( shader.id, 1, &sc, NULL );

		glCompileShader ( shader.id );

		int success;
		glGetShaderiv ( shader.id, GL_COMPILE_STATUS, &success );

		if ( !success )
		{
			int length;
			glGetShaderiv ( shader.id, GL_INFO_LOG_LENGTH, &length );

			char buffer [ length ];
			glGetShaderInfoLog ( shader.id, length, NULL, buffer );

			std::cout << "SHADER " << shader.id << " : " << buffer << std::endl;

			glDeleteShader ( shader.id );
		}

		glAttachShader ( program_id, shader.id );
	}
}

void ShaderProgram::LinkProgram (  )
{
	glLinkProgram ( program_id );

	int success;
	int length;
	
	glGetProgramiv ( program_id, GL_LINK_STATUS, &success );

	if ( !success )
	{
		glGetProgramiv ( program_id, GL_INFO_LOG_LENGTH, &length );

		char buffer [ length ];

		glGetProgramInfoLog ( program_id, length, NULL, buffer );

		std::cout << "LINK : " << buffer << std::endl;
	}

	glValidateProgram ( program_id );

	glGetProgramiv ( program_id, GL_VALIDATE_STATUS, &success );

	if ( !success )
	{
		glGetProgramiv ( program_id, GL_INFO_LOG_LENGTH, &success );

		char buffer [ length ];

		glGetProgramInfoLog ( program_id, length, NULL, buffer );

		std::cout << "VALIDATION : " << buffer << std::endl;
	}
}

void ShaderProgram::UseProgram (  )
{
	glUseProgram ( program_id );
}

unsigned int ShaderProgram::GetProgramID (  )
{
	return program_id;
}
