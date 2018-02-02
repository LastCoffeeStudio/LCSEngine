#ifndef __COMPONENT_H__
#define ____COMPONENT_H__

enum TypeComponent
{
	COMPONENT = 0
};
class GameObject;
class Component
{
public:
	//Don't use component() => Use Component(GameObject*)
	Component();
	Component(GameObject* gameObject);
	~Component();
	virtual bool update();
	virtual void enable();
	virtual void disable();

public:
	bool isEnable;
	TypeComponent typeComponent;
private:
	GameObject* gameObject;
};

#endif
