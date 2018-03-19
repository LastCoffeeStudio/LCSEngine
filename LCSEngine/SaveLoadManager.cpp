#include "SaveLoadManager.h"
#include <fstream>
#include <json.hpp>


SaveLoadManager::SaveLoadManager() {}


SaveLoadManager::~SaveLoadManager() {}

void SaveLoadManager::loadScene(const char* path)
{
	
}

void SaveLoadManager::saveScene(const char* path)
{
	nlohmann::json scenejson;
	//Get List of game objects
	//scenejson ["GameObjects"] =  mainScene.getGameObjectJSON()

	std::ofstream out("pretty.json");
	out << std::setw(4) << scenejson << std::endl;
}
