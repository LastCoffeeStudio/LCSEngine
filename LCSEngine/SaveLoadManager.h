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
	static void convertVectorF3ToMyJSON(const vector<float3>& vectorF, nlohmann::json& structVectorF);
	static void convertMyJSONtoVectorF3(const  nlohmann::json& jsonVectorF, vector<float3>& vectorF);
	static void convertVectorF2ToMyJSON(const vector<float2>& vectorF, nlohmann::json& structVectorF);
	static void convertMyJSONtoVectorF2(const  nlohmann::json& jsonVectorF, vector<float2>& vectorF);
	static void convertVectorUIToMyJSON(const vector<unsigned int>& vectorF, nlohmann::json& structVectorF);
	static void convertMyJSONtoVectorUI(const  nlohmann::json& jsonVectorF, vector<unsigned int>& vectorF);
};

#endif // __SAVELOADMANAGER_H__
