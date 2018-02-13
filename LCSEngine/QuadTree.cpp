#include "Globals.h"
#include "QuadTree.h"
#include "MathGeoLib/src/Geometry/AABB.h"
#include "GameObject.h"
#include "SDL/include/SDL_assert.h"
#include <queue>

QuadTree::QuadTree() {}

QuadTree::~QuadTree() {}

void QuadTree::create(AABB limits)
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

void QuadTree::insertAll(const std::vector<GameObject*>& gameObjects) {}

void QuadTree::remove(GameObject* gameObject) {}

template<typename T>
inline void QuadTree::intersect(std::vector<GameObject*> &resultGameObjects, const T& primitive)
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
	AABB newAABB;
	nodeToCheck->children[0] = new QuadNode();
	newAABB = nodeToCheck->limit;
	newAABB.minPoint = { 0, nodeToCheck->limit.MinY(), 0 };
	nodeToCheck->children[0]->limit = newAABB;

	nodeToCheck->children[1] = new QuadNode();
	newAABB = nodeToCheck->limit;
	newAABB.maxPoint = { 0, nodeToCheck->limit.MaxY(), 0 };
	nodeToCheck->children[1]->limit = newAABB;

	nodeToCheck->children[2] = new QuadNode();
	newAABB.maxPoint = { 0, nodeToCheck->limit.MaxY(),  nodeToCheck->limit.MaxZ() };
	newAABB.minPoint = { nodeToCheck->limit.MinX(), nodeToCheck->limit.MinY(), 0 };
	nodeToCheck->children[2]->limit = newAABB;

	nodeToCheck->children[3] = new QuadNode();
	newAABB.maxPoint = { nodeToCheck->limit.MaxY(), nodeToCheck->limit.MaxY(), 0 };
	newAABB.minPoint = { 0, nodeToCheck->limit.MinY(), nodeToCheck->limit.MinZ() };
	nodeToCheck->children[3]->limit = newAABB;

}

QuadNode* QuadTree::getChildToPutGameObject(GameObject* gameObject, QuadNode* nodeToCheck)
{
	QuadNode* ret = nullptr;
	for(vector<QuadNode*>::iterator it = nodeToCheck->children.begin(); it != nodeToCheck->children.end(); ++it)
	{
		if((*it)->limit.Contains(gameObject->aabb))
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
