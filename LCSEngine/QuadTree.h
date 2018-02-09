#ifndef __QUADTREE_H__
#define __QUADTREE_H__
#include <list>
#include <vector>
#include "MathGeoLib/src/Geometry/AABB.h"

class GameObject;
struct QuadNode
{
	std::list<GameObject*> myGameObjects;
	std::vector<QuadNode*> children = std::vector<QuadNode*>(4, nullptr);
	math::AABB limit;
};

class QuadTree
{
public:
	QuadTree();
	QuadTree(math::AABB limits);
	~QuadTree();
	virtual void clear();
	virtual void insert(GameObject* gameObject);
	virtual void insertAll(const std::vector<GameObject*> &gameObjects);
	virtual void remove(GameObject*); //OPTIONAL NOT USED
	virtual void intersect(std::vector<GameObject*> &resultGameObjects /*,Primitive*/);
	
public:

private:
	QuadNode root;
	
};

#endif //__QUADTREE_H__

