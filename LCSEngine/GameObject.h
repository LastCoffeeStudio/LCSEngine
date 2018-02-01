#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <string>
#include <vector>

using namespace std;

//class Component;
class Transform;

class GameObject
{
public:
	GameObject();
	~GameObject();

	//void addComponent(Component* component);
	//void deleteComponent(Component* component);
	void addGameObject(GameObject* gameObject);
	void deleteGameObject(GameObject* gameObject);
	void drawGui();
	void draw();

public:
	char name[64] = "GameObject";
	//Transform* transform;
	bool enable = true;
	//vector<Component*> components;
	vector<GameObject*> childs;
	GameObject* parent = nullptr;
};

#endif // __GAMEOBJECT_H__