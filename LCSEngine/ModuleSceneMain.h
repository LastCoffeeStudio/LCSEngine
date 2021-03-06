#ifndef __MODULESCENEMAIN_H__
#define __MODULESCENEMAIN_H__

#include "Module.h"
#include "MathGeoLib/src/Geometry/AABB.h"
#include <map>
#include "SceneManager.h"

class Shader;
class AssetTexture;
class GameObject;
class Component;
class ElementGameUI;
class QuadTree;
typedef enum defaultShaders;

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
	void clearElements();
	void preUpdateGameObjects();
	void postUpdateGameObjects();
	void draw();
	void drawGameObjects(GameObject* gameObject);
	void drawGrid();
	void swapDefaultShader();
	void checkVisibleItems();
	void drawQuadTree();
	void makeQuadTree();
	void saveScene(std::string fileName = "");
	void loadScene(std::string fileName = "");

public:
	AssetTexture * checkers = nullptr;
	AssetTexture* lenna = nullptr;
	AssetTexture* chocobo = nullptr;
	AssetTexture* beer = nullptr;
	AssetTexture* actual = nullptr;
	AssetTexture* play = nullptr;
	AssetTexture* pause = nullptr;
	SceneManager* sceneManager;
	Shader* shader = nullptr;
	GameObject* root = nullptr;
	GameObject* currentObject = nullptr;
	QuadTree* quadtree = nullptr;
	bool drawZbuffer = false;
	bool rebuildQuadTree = false;
	std::list<GameObject*> garbageCollector;
	std::list<Component*> garbageCollectorComponent;
	std::list<ElementGameUI*> garbageCollectorElements;
	std::vector<GameObject*> rootBones;
	bool isPlaying = false;

private:
	void drawAABB(const AABB& aabb);
	void changeShaders();
	void modifyShader(defaultShaders shader);
};
#endif //__MODULESCENEMAIN_H__
