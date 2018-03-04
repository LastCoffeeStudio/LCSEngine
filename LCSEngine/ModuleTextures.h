#ifndef __MODULETEXTURES_H__
#define __MODULETEXTURES_H__

#include "Module.h"
#include "Globals.h"
#include <list>
#include <map>
#include <string>

typedef unsigned int ILenum;
typedef unsigned char GLubyte;
typedef unsigned int GLuint;
struct SDL_Texture;
class AssetTexture;
struct aiScene;

class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool init();
	bool cleanUp();

	bool const loadTexture(const char* path);
	void const loadModelTextures(const aiScene* scene);
	AssetTexture* const loadCheckers();

public:
	GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	GLuint ImageName;
	std::map<std::string, AssetTexture*> textures;
};

#endif // __MODULETEXTURES_H__