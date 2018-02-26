#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_
#include <vector>
#include "MathGeoLib/src/Math/float3.h"
#include "Model.h"

typedef unsigned int GLuint;
class aiScene;
class AssetTexture;

class SceneManager
{
public:
    SceneManager();
    ~SceneManager();

    void Load(const char* file);
    void Clear();
public:
    const aiScene* scene = 0;



public:
    std::vector<Model> models;
    std::vector<std::string> filesPath;
};

#endif //_SCENEMANAGER_H_
