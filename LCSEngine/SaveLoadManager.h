#ifndef __SAVELOADMANAGER_H__
#define __SAVELOADMANAGER_H__

#include "GameObject.h"

class SaveLoadManager
{
public:
	SaveLoadManager();
	~SaveLoadManager();

	void loadScene(const char* path, GameObject* root);
	void saveScene(const char* path, GameObject* root);

	static void convertFloat4x4ToMyJSON(const float4x4& basicFloat, nlohmann::json& structFloat);
	static void convertMyJSONtoFloat4x4(const  nlohmann::json& jsonFloat, float4x4& basicFloat);
	static void convertFloat3ToMyJSON(const float3& basicFloat, nlohmann::json& structFloat);
	static void convertMyJSONtoFloat3(const  nlohmann::json& jsonFloat, float3& basicFloat);
};

#endif // __SAVELOADMANAGER_H__
