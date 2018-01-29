#ifndef __ASSETTEXTURE_H__
#define __ASSETTEXTURE_H__
#include "Glew/include/glew.h"
#include <string>

class ILinfo;

typedef unsigned int ILenum;

class AssetTexture
{
public:
	AssetTexture();
	AssetTexture(ILinfo info);
	~AssetTexture();

public:
	std::string name;
	unsigned int width;
	unsigned int height;
	unsigned int depth;
	unsigned int bpp;
	unsigned int mips;
	unsigned int bytes;
	ILenum format = 0;
	GLuint ID = 0;
};
#endif //__ASSETTEXTURE_H__
