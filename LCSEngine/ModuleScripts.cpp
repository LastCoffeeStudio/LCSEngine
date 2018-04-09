#include "Globals.h"
#include "Application.h"
#include "ModuleScripts.h"
#include "ScriptComponent.h"
#include "ModuleSceneMain.h"
#include "ModuleInput.h"
#include "GameObject.h"
#include <vector>

ModuleScripts::ModuleScripts() {}

ModuleScripts::~ModuleScripts() {}

void instantiate() {
	App->sceneMain->root->addGameObject(new GameObject(App->sceneMain->currentObject, "GameObject", uuid()));
}

bool ModuleScripts::init()
{
	lua.open_libraries(sol::lib::base);
	lua.set_function("instantiate", instantiate);
	lua.set_function("getButtonDown", ModuleInput::getButtonDown);

	lua.new_usertype<GameObject>("Gameobject",
		sol::constructors<GameObject()>()
		);

	return true;
}

update_status ModuleScripts::update(float deltaTime)
{
	//for (std::vector<ScriptComponent*>::iterator it = scripts.begin(); it != scripts.end(); ++it)
	//{
		sol::function fx = lua["update"];
		if (fx)
		{
			std::function<void()> stdfx = fx;
			stdfx();
		}
	//}

	return UPDATE_CONTINUE;
}

bool ModuleScripts::cleanUp()
{
	return true;
}

void ModuleScripts::load_script(std::string path)
{
	lua.script_file(path);
}
