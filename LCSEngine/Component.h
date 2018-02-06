#ifndef __COMPONENT_H__
#define __COMPONENT_H__
#include <string>
#include "Globals.h"


class GameObject;
class Component
{
public:
	Component();
	Component(GameObject* gameObject, bool isEnable = true, bool isUnique = false);
	~Component();
	virtual bool update();
	virtual void enable();
	virtual void disable();
	virtual void drawGUI() = 0;

public:
	bool isEnable = false;
	bool isUnique = false;
	TypeComponent typeComponent = COMPONENT;
	bool suicide = false;

protected:
	GameObject* gameObject = nullptr;
};

#endif
