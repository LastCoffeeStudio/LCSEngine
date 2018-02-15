#include "Globals.h"
#include "QuadTree.h"
#include "MathGeoLib/src/Geometry/AABB.h"
#include "GameObject.h"
#include "SDL/include/SDL_assert.h"
#include <queue>

QuadTree::QuadTree() {}

QuadTree::~QuadTree() {}

void QuadTree::create(const AABB &limits)
{
	clear();
	root = new QuadNode();
	root->limit = limits;
}

void QuadTree::clear()
{
	if (root != nullptr)
	{
		RELEASE(root);
	}
}

void QuadTree::insert(GameObject* gameObject)
{
	SDL_assert(gameObject != nullptr);
	list<GameObject*> objectsToInsert;
	objectsToInsert.push_back(gameObject);
	QuadNode* nodeToCheck = root;

	while(nodeToCheck != nullptr)
	{
		if(nodeToCheck->myGameObjects.empty() == true && nodeToCheck->children[0] == nullptr)
		{
			nodeToCheck->myGameObjects.push_back(objectsToInsert.front());
			objectsToInsert.pop_front();
		}
		else if(nodeToCheck->myGameObjects.empty() == false && nodeToCheck->children[0] == nullptr)
		{
			//Si el nodo tiene 1 objecto, tendre que añadir 2 objetos (el nuevo mas el que tiene el nodo).
			//Creo los hijos, añado un objeto, me mantengo en este nodo porque me queda añadir el otro objeto.
			inizialiceChildrens(nodeToCheck);
			objectsToInsert.push_back(nodeToCheck->myGameObjects.front());
			nodeToCheck->myGameObjects = vector<GameObject*>();
			QuadNode* nodeToPut = getChildToPutGameObject(objectsToInsert.front(), nodeToCheck);
			SDL_assert(nodeToPut != nullptr);
			nodeToPut->myGameObjects.push_back(objectsToInsert.front());
			objectsToInsert.pop_front();
		}
		else
		{
			QuadNode* nodeToPut = getChildToPutGameObject(objectsToInsert.front(), nodeToCheck);
			SDL_assert(nodeToPut != nullptr);

			if(nodeToPut == nodeToCheck)
			{
				nodeToCheck->myGameObjects.push_back(objectsToInsert.front());
				objectsToInsert.pop_front();
			}
			else
			{
				nodeToCheck = nodeToPut;
			}
		}
		if(objectsToInsert.empty() == true)
		{
			nodeToCheck = nullptr;
		}
	}

}

void QuadTree::insertAll(list<GameObject*>& gameObjects)
{
	float3 min;
	float3 max;
	list<GameObject*>::iterator it = gameObjects.begin();

	if(it != gameObjects.end())
	{
		min = (*it)->obb.CornerPoint(0);
		max = (*it)->obb.CornerPoint(7);
	}else
	{
		return;
	}

	for(; it != gameObjects.end(); ++it)
	{
		min = Min(min, (*it)->obb.CornerPoint(0));
		max = Max(max, (*it)->obb.CornerPoint(7));
	}
	AABB newLimit = AABB(min, max);
	create(newLimit);

	it = gameObjects.begin();
	for (; it != gameObjects.end(); ++it)
	{
		insert(*it);
	}
}

void QuadTree::remove(GameObject* gameObject) {}

template<typename T>
inline void QuadTree::intersect(vector<GameObject*> &resultGameObjects, const T& primitive)
{
	queue<QuadNode*> children;

	children.push(root);

	while (!children.empty())
	{
		QuadNode* node = children.front();
		SDL_assert(node != nullptr);
		children.pop();
		if (primitive.Intersects(node->limit))
		{
			for (vector<GameObject*>::iterator it = node->myGameObjects.begin(); it != node->myGameObjects.end(); ++it)
			{
				if (primitive.Intersects((*it)->obb.MinimalEnclosingAABB()))
				{
					resultGameObjects.push_back(*it);
				}
			}

			for (vector<QuadNode*>::iterator it = node->children.begin(); it != node->children.end(); ++it)
			{
				if ((*it) != nullptr)
				{
					children.push(*it);
				}
			}
		}
	}
}

void QuadTree::inizialiceChildrens(QuadNode* nodeToCheck)
{
	float3 centerPoint = nodeToCheck->limit.CenterPoint();
	AABB newAABB;
	nodeToCheck->children[0] = new QuadNode();
	newAABB.maxPoint = { nodeToCheck->limit.MaxX(), nodeToCheck->limit.MaxY(), nodeToCheck->limit.MaxZ() };
	newAABB.minPoint = { centerPoint.x, nodeToCheck->limit.MinY(), centerPoint.z };
	nodeToCheck->children[0]->limit = newAABB;

	nodeToCheck->children[1] = new QuadNode();
	newAABB.maxPoint = { centerPoint.x, nodeToCheck->limit.MaxY(), centerPoint.y };
	newAABB.minPoint = { nodeToCheck->limit.MinX(), nodeToCheck->limit.MinY(),nodeToCheck->limit.MinZ() };
	nodeToCheck->children[1]->limit = newAABB;

	nodeToCheck->children[2] = new QuadNode();
	newAABB.maxPoint = { centerPoint.x, nodeToCheck->limit.MaxY(), nodeToCheck->limit.MaxZ() };
	newAABB.minPoint = { nodeToCheck->limit.MinX(), nodeToCheck->limit.MinY(), centerPoint.z };
	nodeToCheck->children[2]->limit = newAABB;

	nodeToCheck->children[3] = new QuadNode();
	newAABB.maxPoint = { nodeToCheck->limit.MaxX(), nodeToCheck->limit.MaxY(), centerPoint.z };
	newAABB.minPoint = { centerPoint.x, nodeToCheck->limit.MinY(), nodeToCheck->limit.MinZ() };
	nodeToCheck->children[3]->limit = newAABB;
	
}

QuadNode* QuadTree::getChildToPutGameObject(GameObject* gameObject, QuadNode* nodeToCheck)
{
	QuadNode* ret = nullptr;
	for(vector<QuadNode*>::iterator it = nodeToCheck->children.begin(); it != nodeToCheck->children.end(); ++it)
	{
		if((*it)->limit.Contains(gameObject->obb) || (*it)->limit.Intersects(gameObject->obb))
		{
			if(ret == nullptr)
			{
				ret = *it;
			}
			else
			{
				return nodeToCheck;
			}
		}
	}
	return ret;
}
