#ifndef __APPLICATION_CPP__
#define __APPLICATION_CPP__

#include <list>
#include "Globals.h"
#include "Module.h"
#include <time.h>
#include "ModuleIntro.h"

class ModuleRender;
class ModuleWindow;
class ModuleJson;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModuleFadeToBlack;

class Application
{
public:
	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleJson* json;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleFadeToBlack* fade;
	ModuleIntro* intro;

private:
	std::list<Module*> modules;
	clock_t timer;
	float deltaTime;
};

extern Application* App;

#endif // __APPLICATION_CPP__