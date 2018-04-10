#pragma once
#include "Module.h"

#include <lua.hpp>
#pragma comment( lib, "Lua/libx86/lua53.lib" )
#include <sol.hpp>

class ScriptComponent;

class ModuleScripts : public Module
{
public:
	ModuleScripts();
	~ModuleScripts();

	bool init() override;
	bool cleanUp() override;

	void updateScript(std::string path);

public:
	sol::state lua;
	std::vector<ScriptComponent*> scripts;
};

