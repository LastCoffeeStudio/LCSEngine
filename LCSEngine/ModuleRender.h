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

	bool Init();
	update_status PreUpdate(const float deltaTime);
	update_status Update(const float deltaTime);
	update_status PostUpdate(const float deltaTime);
	bool CleanUp();

public:
	SDL_Renderer* renderer = nullptr;
};

#endif // __MODULERENDER_H__