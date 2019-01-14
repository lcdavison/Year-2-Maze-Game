#pragma once

#include <glew.h>

/*
	The shader data structure contains data required to load and create shaders.
*/
struct Shader 
{
	GLuint id;		//	OpenGL Shader ID
	GLenum type;		//	OpenGL Shader Type
	const char* path;	//	Path to the shader code
};
