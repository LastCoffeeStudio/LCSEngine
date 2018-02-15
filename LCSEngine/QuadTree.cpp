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
	QuadNode* nodeToCheck = root;
	GameObject* gameObjectToInsert = gameObject;
	list<pair<GameObject*, QuadNode*> > gameObjectNodeList;
	gameObjectNodeList.push_front({ gameObject, root });

	//while list Pair not empty
	while (gameObjectNodeList.empty() == false)
	{
		gameObjectToInsert = gameObjectNodeList.front().first;
		nodeToCheck = gameObjectNodeList.front().second;

		if (nodeToCheck->myGameObjects.size() <= numGameObjectInNode && nodeToCheck->children[0] == nullptr)
		{
			nodeToCheck->myGameObjects.push_back(gameObjectToInsert);
			gameObjectNodeList.pop_front();
		}
		else if(nodeToCheck->myGameObjects.size() > numGameObjectInNode && nodeToCheck->children[0] == nullptr)
		{
			inizialiceChildrens(nodeToCheck);

			for(int i = 0; i < nodeToCheck->myGameObjects.size(); ++i)
			{
				gameObjectNodeList.push_back({ nodeToCheck->myGameObjects[i], nodeToCheck });
			}
			nodeToCheck->myGameObjects = vector<GameObject*>();
		}
		else
		{
			QuadNode* nodeToPut = getChildToPutGameObject(gameObjectToInsert, nodeToCheck);
			SDL_assert(nodeToPut != nullptr);

			if(nodeToPut == nodeToCheck)
			{
				nodeToCheck->myGameObjects.push_back(gameObjectToInsert);
				gameObjectNodeList.pop_front();
			}
			else
			{
				gameObjectNodeList.front().second = nodeToPut;
			}
		}
	}

}

void QuadTree::insertAll(list<GameObject*>& gameObjects)
{
	float minX, minY, minZ;
	float maxX, maxY, maxZ;
	list<GameObject*>::iterator it = gameObjects.begin();

	if(it != gameObjects.end())
	{
		minX = (*it)->obb.CornerPoint(0).x;
		minY = (*it)->obb.CornerPoint(0).y;
		minZ = (*it)->obb.CornerPoint(0).z;
		maxX = (*it)->obb.CornerPoint(7).x;
		maxY = (*it)->obb.CornerPoint(7).y;
		maxZ = (*it)->obb.CornerPoint(7).z;
	}else
	{
		return;
	}

	for(; it != gameObjects.end(); ++it)
	{
		minX = MIN(minX, (*it)->obb.CornerPoint(0).x);
		minY = MIN(minY, (*it)->obb.CornerPoint(0).y);
		minZ = MIN(minZ, (*it)->obb.CornerPoint(0).z);
		maxX = MAX(maxX, (*it)->obb.CornerPoint(7).x);
		maxY = MAX(maxY, (*it)->obb.CornerPoint(7).y);
		maxZ = MAX(maxZ, (*it)->obb.CornerPoint(7).z);
	}
	AABB newLimit = AABB({ minX, minY, minZ }, { maxX, maxY, maxZ});
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
	newAABB.maxPoint = { centerPoint.x, nodeToCheck->limit.MaxY(), centerPoint.z };
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
