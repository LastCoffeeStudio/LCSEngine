#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__


#include <vector>

using namespace std;

class Component;
class Transform;

class GameObject
{
public:
	GameObject();
	GameObject(GameObject* parent, string name);
	~GameObject();

	void addComponent(Component* component);
	void deleteComponent(Component* component);
	void addGameObject(GameObject* gameObject);
	void deleteGameObject();
	void drawGui();
	void draw();

private:
	void deleteChildren();
	void deleteSelf();

public:
	string name = "GameObject";
	bool enable = true;
	vector<Component*> components;
	vector<GameObject*> children;
	GameObject* parent = nullptr;
};

#endif // __GAMEOBJECT_H__