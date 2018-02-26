#include "SceneManager.h"
#include <postprocess.h>
#include <cimport.h>
#include <SDL_assert.h>

SceneManager::SceneManager()
{
    filesPath.reserve(0);
}


SceneManager::~SceneManager()
{
}


void SceneManager::Load(const char* file)
{
    SDL_assert(file == nullptr);
    bool exists = false;
    int index = -1;
    for (int i = 0; i < filesPath.size(); ++i)
    {
        if(file == filesPath[i])
        {
            exists = true;
            index = i;
        }
    }
    if(!exists)
    {
        scene = aiImportFile(file, aiProcessPreset_TargetRealtime_MaxQuality);
        index = filesPath.size() - 1;
    }
    
}
