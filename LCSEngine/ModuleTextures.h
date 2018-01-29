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

	AssetTexture* const loadTexture(ILenum type, const char* path);
	AssetTexture* const loadCheckers();

	GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	GLuint ImageName;

private:
	std::list<SDL_Texture*> textures;
};

#endif // __MODULETEXTURES_H__