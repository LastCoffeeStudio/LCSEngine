#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleJson.h"
#include "ModuleRender.h"
#include "ModuleGUI.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneMain.h"
#include "ModuleCamera.h"
#include "ModuleAnimation.h"
#include "ModuleGameUI.h"
#include "ModuleType.h"
#include "ModuleScripts.h"

using namespace std;

Application::Application()
{
	// Order matters: they will init/start/pre/update/post in this order
	modules.push_back(input = new ModuleInput());
	modules.push_back(window = new ModuleWindow());
	modules.push_back(json = new ModuleJson());
	modules.push_back(camera = new ModuleCamera());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(gameUI = new ModuleGameUI());
	modules.push_back(gui = new ModuleGUI());
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(audio = new ModuleAudio());
	modules.push_back(fade = new ModuleFadeToBlack());
	modules.push_back(animations = new ModuleAnimation());
	modules.push_back(sceneMain = new ModuleSceneMain(false));
	modules.push_back(type = new ModuleType());
	modules.push_back(scripts = new ModuleScripts());

	timer = 0;
	deltaTime = 0;
}

Application::~Application()
{
	for (list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
	{
		RELEASE(*it);
	}
}

bool Application::init()
{
	bool ret = true;

	for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		ret = (*it)->init(); // we init everything, even if not enabled
	}

	for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		if ((*it)->isEnabled() == true)
		{
			ret = (*it)->start();
		}
	}

	// Start the first scene --
	sceneMain->enable();

	return ret;
}

update_status Application::update()
{
	update_status ret = UPDATE_CONTINUE;

	clock_t now = clock();
	deltaTime += float(now - timer) / CLOCKS_PER_SEC;
	if (deltaTime > 1 / FPS) 
	{
		timer = now;
		for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		{
			if ((*it)->isEnabled() == true)
			{
				ret = (*it)->preUpdate(deltaTime);
			}
		}

		for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		{
			if ((*it)->isEnabled() == true)
			{
				ret = (*it)->update(deltaTime);
			}
		}

		for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		{
			if ((*it)->isEnabled() == true)
			{
				ret = (*it)->postUpdate(deltaTime);
			}
		}

		deltaTime = 0.f;
	}
	return ret;
}

bool Application::cleanUp()
{
	bool ret = true;

	for (list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
	{
		if ((*it)->isEnabled() == true)
		{
			ret = (*it)->cleanUp();
		}
	}

	return ret;
}

