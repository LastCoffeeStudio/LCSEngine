#ifndef __MODULETEXTURES_H__
#define __MODULETEXTURES_H__

#include<list>
#include "Module.h"
#include "Globals.h"
#include "Glew/include/glew.h"

struct SDL_Texture;

class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool init();
	bool cleanUp();

	SDL_Texture* const load(const char* path);
	void loadCheckers();
	void unload(SDL_Texture* texture);

	GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	GLuint ImageName;
private:
	std::list<SDL_Texture*> textures;
};

#endif // __MODULETEXTURES_H__