#ifndef __APPLICATION_CPP__
#define __APPLICATION_CPP__

#include <list>
#include "Globals.h"
#include "Module.h"
#include <time.h>

class ModuleRender;
class ModuleWindow;
class ModuleJson;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModuleFadeToBlack;
class ModuleSceneMain;
class ModuleGUI;
class ModuleCamera;
class ModuleAnimation;
class ModuleGameUI;
class ModuleType;

class Application
{
public:
	Application();
	~Application();

	bool init();
	update_status update();
	bool cleanUp();

public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleJson* json;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleFadeToBlack* fade;
	ModuleSceneMain* sceneMain;
	ModuleGUI* gui;
	ModuleCamera* camera;
	ModuleAnimation* animations;
	ModuleGameUI* gameUI;
	ModuleType* type;

private:
	std::list<Module*> modules;
	clock_t timer;
	float deltaTime;
};

extern Application* App;

#endif // __APPLICATION_CPP__