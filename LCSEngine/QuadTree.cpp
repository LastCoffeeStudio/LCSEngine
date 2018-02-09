#include "QuadTree.h"
#include "MathGeoLib/src/Geometry/AABB.h"
#include "GameObject.h"

QuadTree::QuadTree() {}

QuadTree::QuadTree(AABB limits)
{
	root.limit = limits;
}

QuadTree::~QuadTree()
{
	
}

void QuadTree::clear()
{
	
}

void QuadTree::insert(GameObject* gameObject)
{
	
}

void QuadTree::insertAll(const std::vector<GameObject*>& gameObjects)
{
	
}

void QuadTree::remove(GameObject*)
{
	
}


void QuadTree::intersect(std::vector<GameObject*>& resultGameObjects)
{
	
}





