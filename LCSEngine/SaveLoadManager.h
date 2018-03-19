#ifndef __SAVELOADMANAGER_H__
#define __SAVELOADMANAGER_H__

#include "GameObject.h"

class SaveLoadManager
{
public:
	SaveLoadManager();
	~SaveLoadManager();

	void loadScene(const char* path);
	void saveScene(const char* path, GameObject* root);
	
};

#endif // __SAVELOADMANAGER_H__
