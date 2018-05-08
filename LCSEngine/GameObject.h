#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "glew.h"
#include "Globals.h"
#include "MathGeoLib/src/Math/float4x4.h"
#include "MathGeoLib/src/Geometry/AABB.h"
#include "MathGeoLib/src/Geometry/OBB.h"
#include <vector>
#include <string>
#include <json.hpp>

using namespace std;

class Component;
class ElementGameUI;
class AnimationComponent;
class Transform;
class Model;

class GameObject
{
public:
	GameObject();
	GameObject(GameObject* parent, string name, unsigned int UUID);
	~GameObject();

	void preUpdate();
	void postUpdate();
	void addComponent(Component* component);
	void addElement(ElementGameUI* component);
	vector<Component*>::iterator deleteComponent(Component* component);
	vector<ElementGameUI*>::iterator deleteElement(ElementGameUI* element);
	void addGameObject(GameObject* gameObject);
	void drawComponentsElementsGui();
	void draw();
	void drawAABB();
	void drawOBB();
	void drawFrustum(Frustum frustum);
	void setStaticValueToChildrens();
	void setStaticFlag(bool flag);
	void save(nlohmann::json& conf);
	void load(nlohmann::json& conf);
	vector<Component*> getComponents(TypeComponent type);
	Component* getComponent(TypeComponent type);
	static void instantiate();

public:
	string name = "GameObject";
    unsigned int UUID;
	bool enable = true;
	bool staticFlag = false;
	vector<Component*> components;
	vector<ElementGameUI*> elements;
	vector<GameObject*> children;
	GameObject* parent = nullptr;
    unsigned int UUIDparent = NULL;
	int nameNumber = 0;
	float4x4 id = float4x4::identity;
	float4x4 normalId = float4x4::identity;
	float4x4 idBone = float4x4::identity;
	AABB aabb;
	OBB obb;
	bool visible = true;

private:
	string getFinalName(string name);
	void updateBones(const AnimationComponent* anim);
	void updateTransformBones(const AnimationComponent* anim);
	void updateVertices(const AnimationComponent* anim);
	void updateComponents();
	void updateElements();
	void updateTexture(string& lastPath, const char* newPath, GLuint& id, bool& hasTexture);
	void updateNormalMap(string& lastPath, const char* newPath, GLuint& id, bool& hasTexture);

private:
	string initialName;
	GLuint program = 0;
	GLuint texID = 0;
	std::string texPath = "";
	std::string normalMapPath = "";

	bool staticHasChanged = false;
	bool staticPreviousValue = false;
	bool hasTexture = false;
};

#endif // __GAMEOBJECT_H__