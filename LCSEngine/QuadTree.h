#ifndef __QUADTREE_H__
#define __QUADTREE_H__
#include <list>
#include <vector>
#include "MathGeoLib/src/Geometry/AABB.h"

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
	QuadTree(math::AABB limits);
	~QuadTree();
	void clear();
	void insert(GameObject* gameObject);
	void insertAll(const std::vector<GameObject*> &gameObjects);
	void remove(GameObject*); //OPTIONAL NOT USED
	void intersect(std::vector<GameObject*> &resultGameObjects /*,Primitive*/);
	
public:

private:
	void inizialiceChildrens(QuadNode* nodeToCheck);
	QuadNode* getChildToPutGameObject(GameObject* gameObject, QuadNode* nodeToCheck);
	QuadNode* root;
	
};

#endif //__QUADTREE_H__

