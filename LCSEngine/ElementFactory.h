#ifndef __ElementFactory_H__
#define __ElementFactory_H__

enum TypeComponent;
class ElementGameUI;
class GameObject;

class ElementFactory
{
public:
	~ElementFactory();
	static ElementFactory* getInstance();
	ElementGameUI* getComponent(TypeElemeneGametUI typeElement, GameObject* parentObject, int x, int y, int h, int w, bool isVisible = true);

private:
	ElementFactory();

};

#endif //__ElementFactory_H__