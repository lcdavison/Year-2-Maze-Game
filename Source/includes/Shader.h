#pragma once

#include <glew.h>

struct Shader 
{
	unsigned int id;
	GLenum type;
	const char* path;
};
