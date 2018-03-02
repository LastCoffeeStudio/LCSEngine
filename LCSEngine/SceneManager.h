#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_
#include <vector>
#include "MathGeoLib/src/Math/float3.h"
#include "Model.h"
#include "assimp/include/scene.h"
#include "assimp/include/cimport.h"
#include "assimp/include/postprocess.h" 

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

public:
	std::vector<Model> models;
	std::vector<std::string> filesPath;
};

#endif //_SCENEMANAGER_H_