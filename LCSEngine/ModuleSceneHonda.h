#ifndef __MODULESCENEHONDA_H__
#define __MODULESCENEHONDA_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"

struct SDL_Texture;

class ModuleSceneHonda : public Module
{
public:
	ModuleSceneHonda(bool start_enabled = true);
	~ModuleSceneHonda();

	bool Start();
	update_status Update();
	bool CleanUp();

public:

	SDL_Texture* graphics = nullptr;
	SDL_Texture* graphicsBackground = nullptr;
	SDL_Rect ground;
	SDL_Rect top;
	SDL_Rect background;
	SDL_Rect foreground;
	Animation sun;
	Animation man;
	Animation water;
	unsigned int hondaMusicId;
};

#endif // __MODULESCENEHONDA_H__