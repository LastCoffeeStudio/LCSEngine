#include "Globals.h"
#include "Application.h"
#include "ModuleScripts.h"
#include "ScriptComponent.h"
#include "ModuleSceneMain.h"
#include "ModuleInput.h"
#include "GameObject.h"
#include <iostream>
#include <vector>

ModuleScripts::ModuleScripts() {}

ModuleScripts::~ModuleScripts() {}

void instantiate() {
	GameObject* gameObject = new GameObject(App->sceneMain->currentObject, "GameObject", uuid());
	App->sceneMain->root->addGameObject(gameObject);
}

bool ModuleScripts::init()
{
	lua.open_libraries(sol::lib::base);
	lua.set_function("instantiate", instantiate);

	lua.new_usertype<GameObject>("Gameobject",
		sol::constructors<GameObject()>(),
		"visible", &GameObject::visible
		);

	lua.new_usertype<ModuleInput>("Input",
		"getButtonDown", &ModuleInput::getButtonDown
		);

	return true;
}

bool ModuleScripts::cleanUp()
{
	return true;
}

void ModuleScripts::updateScript(std::string path)
{
	if (!path.empty())
	{
		lua.script_file(path);
		sol::function fx = lua["update"];
		if (fx)
		{
			std::function<void()> stdfx = fx;
			stdfx();
		}
	}
}

void ModuleScripts::addGameObject(GameObject * gameObject)
{
	lua["gameObject"] = gameObject;
}
