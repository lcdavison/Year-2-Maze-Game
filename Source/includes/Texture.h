#pragma once

enum TEXTURE_TYPE { DIFFUSE = 0x1, SPECULAR = 0x2 };

struct Texture 
{
	unsigned int texture_id;
	TEXTURE_TYPE type;
};
