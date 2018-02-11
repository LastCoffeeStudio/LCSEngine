#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "glew.h"
#include "MathGeoLib/src/Math/float4x4.h"
#include "MathGeoLib/src/Geometry/AABB.h"
#include "MathGeoLib/src/Geometry/OBB.h"
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

	void preUpdate();
	void addComponent(Component* component);
	void deleteComponent(Component* component);
	void addGameObject(GameObject* gameObject);
	void drawComponentsGui();
	void draw();
	void drawAABB();
	void drawOBB();
	void drawFrustum(Frustum frustum);

public:
	string name = "GameObject";
	bool enable = true;
	bool staticFlag = false;
	vector<Component*> components;
	vector<GameObject*> children;
	GameObject* parent = nullptr;
	int nameNumber = 0;
	float4x4 id;
	AABB aabb;
	OBB obb;
	bool suicide = false;
	bool visible = true;

private:
	string getFinalName(string name);
	string initialName;
	GLint idVertVBO = -1;
	unsigned int sizeVertVBO = 0;
};

#endif // __GAMEOBJECT_H__