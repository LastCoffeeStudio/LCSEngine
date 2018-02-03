#ifndef __COMPONENT_H__
#define __COMPONENT_H__
#include <string>

enum TypeComponent
{
	COMPONENT = 0,
	TRANSFORM,
	MATERIAL,
	MESHCOMPONENT
};

class GameObject;
class Component
{
public:
	Component();
	Component(GameObject* gameObject, bool isEnable = false, bool isUnique = false);
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
