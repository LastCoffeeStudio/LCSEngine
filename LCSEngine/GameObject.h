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
	void drawComponentsGui();
	void draw();

public:
	string name = "GameObject";
	bool enable = true;
	vector<Component*> components;
	vector<GameObject*> children;
	GameObject* parent = nullptr;
	int nameNumber = 0;

private:
	string getFinalName(string name);
	string initialName;
};

#endif // __GAMEOBJECT_H__