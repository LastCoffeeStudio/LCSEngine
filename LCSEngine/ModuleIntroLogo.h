#ifndef __MODULEINTROLOGO_H__
#define __MODULEINTROLOGO_H__

#include "Module.h"

struct SDL_Texture;

class ModuleIntroLogo : public Module
{
public:
	ModuleIntroLogo();
	~ModuleIntroLogo();

	bool Start() override;
	update_status Update(const float deltaTime) override;

public:
	SDL_Texture * background = nullptr;
};
#endif //__MODULEINTROLOGO_H__
