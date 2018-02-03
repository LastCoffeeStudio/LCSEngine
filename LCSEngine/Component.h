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

private:
	GameObject* gameObject = nullptr;
};

#endif
