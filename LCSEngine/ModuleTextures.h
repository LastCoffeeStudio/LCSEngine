#ifndef __MODULETEXTURES_H__
#define __MODULETEXTURES_H__

#include <list>
#include "Module.h"
#include "Globals.h"
#include "Glew/include/glew.h"

struct SDL_Texture;
class AssetTexture;
typedef unsigned int ILenum;

class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool init();
	bool cleanUp();

	//SDL_Texture* const load(const char* path);
	AssetTexture* const load(ILenum type, const char* path);
	AssetTexture* const loadTexture(ILenum type, const char* path);
	AssetTexture* const loadCheckers();

	GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
};

#endif // __MODULETEXTURES_H__