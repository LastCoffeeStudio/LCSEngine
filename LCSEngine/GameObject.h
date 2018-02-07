#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "glew.h"
#include <vector>
#include "MathGeoLib/src/Math/float4x4.h"
#include "MathGeoLib/src/Geometry/AABB.h"
#include "MathGeoLib/src/Geometry/OBB.h"

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
	void drawAABB();
	void drawOBB();
	void drawFrustum(Frustum frustum);

public:
	string name = "GameObject";
	bool enable = true;
	vector<Component*> components;
	vector<GameObject*> children;
	GameObject* parent = nullptr;
	int nameNumber = 0;
	float4x4 id;
	AABB aabb;
	OBB obb;
	bool suicide = false;

private:
	string getFinalName(string name);
	string initialName;

	/*Create map to know if it has a component (updated when component added or deleted)*/

	/*TO DELETE*/
};

#endif // __GAMEOBJECT_H__