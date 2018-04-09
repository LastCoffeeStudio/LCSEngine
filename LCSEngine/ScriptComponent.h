#ifndef __SCRIPTCOMPONENT_H__
#define __SCRIPTCOMPONENT_H__

#include "Component.h"

#include <lua.hpp>
#pragma comment( lib, "Lua/libx86/lua53.lib" )
#include <sol.hpp>

class ScriptComponent : public Component
{
public:
	ScriptComponent(GameObject* gameObject);
	~ScriptComponent();
	void drawGUI() override;

	sol::state lua;
	void updateScripts();

	std::string scriptPath;


};

#endif

