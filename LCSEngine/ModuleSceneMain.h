#ifndef __MODULESCENEMAIN_H__
#define __MODULESCENEMAIN_H__

#include "Module.h"
#include "MathGeoLib/src/Geometry/AABB.h"
#include <map>

class Shader;
class AssetTexture;
class GameObject;
class Component;
class QuadTree;

class ModuleSceneMain : public Module
{
public:
	ModuleSceneMain(bool active = true);
	~ModuleSceneMain();

	bool init() override;
	bool start() override;
	update_status preUpdate(float deltaTime);
	update_status update(float deltaTime);
	update_status postUpdate(float deltaTime);
	bool cleanUp() override;
	void clearGameObjects(); 
	void clearComponents();
	void preUpdateGameObjects();
	void postUpdateGameObjects();
	void draw();
	void drawGameObjects(GameObject* gameObject);
	void drawGrid();
	void swapDefaultShader();
	void checkVisibleItems();
	void drawQuadTree();
	void makeQuadTree();

public:
	AssetTexture * checkers = nullptr;
	AssetTexture* lenna = nullptr;
	AssetTexture* chocobo = nullptr;
	AssetTexture* beer = nullptr;
	AssetTexture* actual = nullptr;
	Shader* shader = nullptr;
	GameObject* root = nullptr;
	GameObject* currentObject = nullptr;
	QuadTree* quadtree = nullptr;
	bool drawZbuffer = false;
	bool rebuildQuadTree = false;
	std::list<GameObject*> garbageCollector;
	std::list<Component*> garbageCollectorComponent;

private:
	void drawAABB(const AABB& aabb);
};
#endif //__MODULESCENEMAIN_H__
