#ifndef __COMPONENTFACTORY_H__
#define __COMPONENTFACTORY_H__

enum TypeComponent;
class Component;
class GameObject;

class ComponentFactory
{
public:
	~ComponentFactory();
	static ComponentFactory* getInstance();
	Component* getComponent(TypeComponent typeComponent, GameObject* parentObject);
	
private:
	ComponentFactory();
	
};

#endif //__COMPONENTFACTORY_H__