#ifndef __ASSETTEXTURE_H__
#define __ASSETTEXTURE_H__
#include "Glew/include/glew.h"

class ILinfo;

typedef unsigned int ILenum;

class AssetTexture
{
public:
	AssetTexture();
	AssetTexture(ILinfo info);
	~AssetTexture();

public:
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int depth = 0;
	ILenum format = 0;
	GLuint ID = 0;
};
#endif //__ASSETTEXTURE_H__
