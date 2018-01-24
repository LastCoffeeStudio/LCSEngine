#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include "Globals.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool init();
	update_status preUpdate(const float deltaTime);
	update_status update(const float deltaTime);
	update_status postUpdate(const float deltaTime);
	bool cleanUp();

	void  updatedWindowSize(int screenWidth, int screenHeight);

public:
	SDL_Renderer* renderer = nullptr;
};

#endif // __MODULERENDER_H__