#ifndef __SCRIPTCOMPONENT_H__
#define __SCRIPTCOMPONENT_H__

#include "Component.h"

class ScriptComponent : public Component
{
public:
	ScriptComponent(GameObject* gameObject);
	~ScriptComponent();
	void drawGUI() override;

public:
	std::string scriptPath;
};

#endif

