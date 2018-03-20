#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "Globals.h"
#include <json.hpp>

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
	virtual void load(nlohmann::json& conf);
	virtual void save(nlohmann::json& conf);

public:
	bool isEnable = false;
	bool isUnique = false;
    unsigned int UUIDparent = NULL;
	TypeComponent typeComponent = COMPONENT;
	GameObject* gameObject = nullptr;

};

#endif //__COMPONENT_H__
