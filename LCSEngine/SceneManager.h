#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include "MathGeoLib/src/Math/float3.h"
#include "Model.h"
#include "assimp/include/scene.h"
#include "assimp/include/cimport.h"
#include "assimp/include/postprocess.h" 
#include <vector>
#include <map>

typedef unsigned int GLuint;

class GameObject;
class AssetTexture;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void load(const char* file);
	GameObject* createObject(GameObject * gameobject, aiNode* parentNode);
	void clear();

public:
	const aiScene* scene = 0;
	std::vector<Model> models;
	std::vector<std::string> filesPath;

private:
	void fillJoints(const GameObject* root, std::map<std::string, GameObject*>& joints);

};

#endif //_SCENEMANAGER_H_