#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <string>

class GameObject;

enum TypeComponent
{
	COMPONENT = 0,
	MESHCOMPONENT,
	TRANSFORMCOMPONENT
};

class Component
{
public:
	//Don't use component() => Use Component(GameObject*)
	Component();
	Component(GameObject* gameObject, std::string name, bool isEnable = false, bool isUnique = false);
	~Component();
	virtual bool update();
	virtual void enable();
	virtual void disable();
	virtual void drawGUI() = 0;

public:
	bool isEnable = false;
	bool isUnique = false;
	std::string name;
	TypeComponent typeComponent = COMPONENT;

protected:
	GameObject* gameObject = nullptr;
};

#endif
