#pragma once

enum TEXTURE_TYPE { DIFFUSE = 0x1, SPECULAR = 0x2 };	//	Used to determine the type of texture

/*
	The texture data structure contains data required to render textures.
*/
struct Texture 
{
	unsigned int texture_id;	//	OpenGL texture ID
	TEXTURE_TYPE type;		//	Texture type
};
