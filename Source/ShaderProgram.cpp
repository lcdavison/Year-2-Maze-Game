#include "Includes/ShaderProgram.h"

ShaderProgram::ShaderProgram (  )
{
	program_id = glCreateProgram (  );	
}

/*
	CreateShader

	Used to create a shader using the code from the file path and the type specified.
*/
void ShaderProgram::CreateShader ( const char* path, GLenum type )
{
	//	Create shader
	Shader new_shader;
	new_shader.type = type;
	new_shader.path = path;

	//	Add to list of shaders
	shaders.push_back ( new_shader );
}

/*
	CompileShaders

	Used to compile the shader code for each of the created shaders.
*/
void ShaderProgram::CompileShaders (  )
{
	//	Compile each shader
	for ( Shader shader : shaders )
	{
		//	Generate shader object
		shader.id = glCreateShader ( shader.type );

		//	Obtain shader code
		std::string shader_code;
		ServiceLocator::LocateResourceManager (  )->LoadShader ( shader.path, shader_code );

		const char* sc = shader_code.c_str (  );

		//	Set shader source code for OpenGL
		glShaderSource ( shader.id, 1, &sc, NULL );

		//	Compile the shader
		glCompileShader ( shader.id );

		//	Determine compile status
		int success;
		glGetShaderiv ( shader.id, GL_COMPILE_STATUS, &success );

		//	Print errors if compile fails
		if ( !success )
		{
			int length;
			glGetShaderiv ( shader.id, GL_INFO_LOG_LENGTH, &length );

			char buffer [ length ];
			glGetShaderInfoLog ( shader.id, length, NULL, buffer );

			std::cout << "SHADER " << shader.id << " : " << buffer << std::endl;

			glDeleteShader ( shader.id );

			return;
		}

		//	Attach the shader to the shader program
		glAttachShader ( program_id, shader.id );
	}
}

/*
	LinkProgram

	Used to link the shader program to create an executable for the GPU, and validates 
	whether the shader program can be executed in OpenGL's current state.
*/
void ShaderProgram::LinkProgram (  )
{
	//	Link program
	glLinkProgram ( program_id );

	//	Determine link status
	int success;
	int length;
	
	glGetProgramiv ( program_id, GL_LINK_STATUS, &success );

	//	Print errors if linking failed
	if ( !success )
	{
		glGetProgramiv ( program_id, GL_INFO_LOG_LENGTH, &length );

		char buffer [ length ];

		glGetProgramInfoLog ( program_id, length, NULL, buffer );

		std::cout << "LINK : " << buffer << std::endl;
	}

	//	Validate program
	glValidateProgram ( program_id );

	//	Determine validate status
	glGetProgramiv ( program_id, GL_VALIDATE_STATUS, &success );

	//	Print errors if validation failed
	if ( !success )
	{
		glGetProgramiv ( program_id, GL_INFO_LOG_LENGTH, &success );

		char buffer [ length ];

		glGetProgramInfoLog ( program_id, length, NULL, buffer );

		std::cout << "VALIDATION : " << buffer << std::endl;
	}
}

/*
	UseProgram

	Used to tell OpenGL to use this shader program for rendering.
*/
void ShaderProgram::UseProgram (  )
{
	glUseProgram ( program_id );
}

/*
	GetProgramID

	Returns the ID that is assigned to this shader program.
*/
unsigned int ShaderProgram::GetProgramID (  )
{
	return program_id;
}
