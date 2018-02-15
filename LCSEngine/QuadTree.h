#ifndef __QUADTREE_H__
#define __QUADTREE_H__
#include <list>
#include <vector>
#include "MathGeoLib/src/Geometry/AABB.h"
#include <queue>

class GameObject;

struct QuadNode
{
	std::vector<GameObject*> myGameObjects;
	std::vector<QuadNode*> children = std::vector<QuadNode*>(4, nullptr);
	math::AABB limit;
};

class QuadTree
{
public:
	QuadTree();
	~QuadTree();
	void create(const math::AABB &limits);
	void clear();
	void insert(GameObject* gameObject);
	void insertAll(std::list<GameObject*> &gameObjects);
	void remove(GameObject* gameObject); //OPTIONAL NOT USED
	template<typename T>
	inline void intersect(std::vector<GameObject*> &resultGameObjects, const T& primitive);
	
public:
	QuadNode* root = nullptr;
	int numGameObjectInNode = 3;

private:
	void inizialiceChildrens(QuadNode* nodeToCheck);
	QuadNode* getChildToPutGameObject(GameObject* gameObject, QuadNode* nodeToCheck);
	
};

#endif //__QUADTREE_H__

