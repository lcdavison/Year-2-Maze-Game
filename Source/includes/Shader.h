#pragma once

#include <glew.h>

struct Shader 
{
	GLuint id;
	GLenum type;
	const char* path;
};
