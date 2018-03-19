#include "SaveLoadManager.h"
#include "GameObject.h"
#include "Globals.h"
#include <queue>
#include <list>
#include <fstream>
#include <json.hpp>


SaveLoadManager::SaveLoadManager() {}


SaveLoadManager::~SaveLoadManager() {}

void SaveLoadManager::loadScene(const char* path)
{
	
}

void SaveLoadManager::saveScene(const char* path, GameObject* root)
{
	if (root == nullptr) {
		LOG("Empty root for save");
	}
	nlohmann::json scenejson;
	list<nlohmann::json> gameObjectsJSON;
	if (root != nullptr)
	{
		std::queue<GameObject*> children;

		for (vector<GameObject*>::iterator it = root->children.begin(); it != root->children.end(); ++it)
		{
			children.push(*it);
		}

		while (!children.empty())
		{
			GameObject* node = children.front();
			children.pop();
			for (vector<GameObject*>::iterator it = node->children.begin(); it != node->children.end(); ++it)
			{
				children.push(*it);
			}
			nlohmann::json gameObjJSON;
			node->save(gameObjJSON);
			gameObjectsJSON.push_back(gameObjJSON);
		}
	}
	scenejson["GameObjects"] = gameObjectsJSON;

	std::ofstream out("Assets/Json/Scene.json");
	out << std::setw(4) << scenejson << std::endl;
}
