#ifndef __COMPONENTFACTORY_H__
#define __COMPONENTFACTORY_H__
#include "Globals.h"
#include "Component.h"

class Component;
class ComponentFactory
{
public:
	~ComponentFactory();
	static ComponentFactory* getInstance();
	Component* getComponent(TypeComponent typeComponent, GameObject* parentObject);
	
private:
	ComponentFactory();
	
};

#endif