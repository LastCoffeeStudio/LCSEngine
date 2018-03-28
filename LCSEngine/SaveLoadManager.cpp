#include "SaveLoadManager.h"
#include "GameObject.h"
#include "Globals.h"
#include <queue>
#include <list>
#include <fstream>
#include <json.hpp>
#include "MathGeoLib/src/Math/float2.h"


SaveLoadManager::SaveLoadManager() {}


SaveLoadManager::~SaveLoadManager() {}

void SaveLoadManager::loadScene(const char* path, GameObject* root)
{
	std::ifstream input("Assets/Json/Scene.json");
	nlohmann::json sceneJson;
	input >> sceneJson;

	list<nlohmann::json> gameObjectsJson = sceneJson["GameObjects"];
	list<GameObject*> gameObjectsScene;
	gameObjectsScene.push_back(root);
	for(list<nlohmann::json>::iterator it = gameObjectsJson.begin(); it != gameObjectsJson.end(); ++it)
	{
		GameObject* gameObj = new GameObject();
		gameObj->load(*it);
		gameObjectsScene.push_back(gameObj);
		bool founded = false;
		for (list<GameObject*>::iterator itObj = gameObjectsScene.begin(); itObj != gameObjectsScene.end() & !founded; ++itObj)
		{
			if(gameObj->UUIDparent == (*itObj)->UUID)
			{
				gameObj->parent = *itObj;
				(*itObj)->children.push_back(gameObj);
				founded = true;
			}
		}
	}
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


void SaveLoadManager::convertFloat4x4ToMyJSON(const float4x4& basicFloat, nlohmann::json& jsonFloat)
{
	jsonFloat = { basicFloat[0][0], basicFloat[0][1],	basicFloat[0][2], basicFloat[0][3],
		basicFloat[1][0], basicFloat[1][1],	basicFloat[1][2], basicFloat[1][3],
		basicFloat[2][0], basicFloat[2][1],	basicFloat[2][2], basicFloat[2][3],
		basicFloat[3][0], basicFloat[3][1],	basicFloat[3][2], basicFloat[3][3] };
}

void SaveLoadManager::convertMyJSONtoFloat4x4(const  nlohmann::json& jsonFloat, float4x4& basicFloat)
{
	if(jsonFloat[0].is_null() == false)
	{
		basicFloat[0][0] = jsonFloat[0].get<float>();
		basicFloat[0][1] = jsonFloat[1].get<float>();
		basicFloat[0][2] = jsonFloat[2].get<float>();
		basicFloat[0][3] = jsonFloat[3].get<float>();
		basicFloat[1][0] = jsonFloat[4].get<float>();
		basicFloat[1][1] = jsonFloat[5].get<float>();
		basicFloat[1][2] = jsonFloat[6].get<float>();
		basicFloat[1][3] = jsonFloat[7].get<float>();
		basicFloat[2][0] = jsonFloat[8].get<float>();
		basicFloat[2][1] = jsonFloat[9].get<float>();
		basicFloat[2][2] = jsonFloat[10].get<float>();
		basicFloat[2][3] = jsonFloat[11].get<float>();
		basicFloat[3][0] = jsonFloat[12].get<float>();
		basicFloat[3][1] = jsonFloat[13].get<float>();
		basicFloat[3][2] = jsonFloat[14].get<float>();
		basicFloat[3][3] = jsonFloat[15].get<float>();
	}
}

void SaveLoadManager::convertFloat3ToMyJSON(const float3& basicFloat, nlohmann::json& jsonFloat)
{
	jsonFloat = { basicFloat.x, basicFloat.y,	basicFloat.z };
}

void SaveLoadManager::convertMyJSONtoFloat3(const  nlohmann::json& jsonFloat, float3& basicFloat)
{
	if (jsonFloat[0].is_null() == false)
	{
		basicFloat.x = jsonFloat[0].get<float>();
		basicFloat.y = jsonFloat[1].get<float>();
		basicFloat.z = jsonFloat[2].get<float>();
	}
}

void SaveLoadManager::convertVectorF3ToMyJSON(const vector<float3>& vectorF, nlohmann::json & structVectorF)
{
	for (int i = 0; i < vectorF.size(); ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			structVectorF.push_back(vectorF[i][j]);
		}
	}
}

void SaveLoadManager::convertMyJSONtoVectorF3(const nlohmann::json & jsonVectorF, vector<float3>& vectorF)
{
	for (int i = 0; i < jsonVectorF.size(); i += 3)
	{
		float3 vertex;
		vertex.x = jsonVectorF[i].get<float>();
		vertex.y = jsonVectorF[i+1].get<float>();
		vertex.z = jsonVectorF[i+2].get<float>();
		
		vectorF.push_back(vertex);
	}
}

void SaveLoadManager::convertVectorF2ToMyJSON(const vector<float2>& vectorF, nlohmann::json & structVectorF)
{
	for (int i = 0; i < vectorF.size(); ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			structVectorF.push_back(vectorF[i][j]);
		}
	}
}

void SaveLoadManager::convertMyJSONtoVectorF2(const nlohmann::json & jsonVectorF, vector<float2>& vectorF)
{
	for (int i = 0; i < jsonVectorF.size(); i += 2)
	{
		float2 vertex;
		vertex.x = jsonVectorF[i].get<float>();
		vertex.y = jsonVectorF[i + 1].get<float>();

		vectorF.push_back(vertex);
	}
}

void SaveLoadManager::convertVectorUIToMyJSON(const vector<unsigned int>& vectorUI, nlohmann::json & structVectorF)
{
	for (int i = 0; i < vectorUI.size(); ++i)
	{
		structVectorF.push_back(vectorUI[i]);
	}
}

void SaveLoadManager::convertMyJSONtoVectorUI(const nlohmann::json & jsonVectorF, vector<unsigned int>& vectorUI)
{
	for (int i = 0; i < jsonVectorF.size(); ++i)
	{
		vectorUI.push_back(jsonVectorF[i]);
	}
}
