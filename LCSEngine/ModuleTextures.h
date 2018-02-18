#ifndef __MODULETEXTURES_H__
#define __MODULETEXTURES_H__

#include "Module.h"
#include "Globals.h"
#include <list>
#include <map>

typedef unsigned int ILenum;
typedef unsigned char GLubyte;
typedef unsigned int GLuint;
struct SDL_Texture;
class AssetTexture;
class aiScene;

class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool init();
	bool cleanUp();

	AssetTexture* const loadTexture(ILenum type, const char* path);
	void const loadModelTextures(const aiScene* scene, std::map<unsigned int, AssetTexture*>& textures);
	AssetTexture* const loadCheckers();

	GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	GLuint ImageName;

private:
	std::list<SDL_Texture*> textures;
};

#endif // __MODULETEXTURES_H__