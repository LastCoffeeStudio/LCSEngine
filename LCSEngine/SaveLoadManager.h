#ifndef __SAVELOADMANAGER_H__
#define __SAVELOADMANAGER_H__

class SaveLoadManager
{
public:
	SaveLoadManager();
	~SaveLoadManager();

	void loadScene(const char* path);
	void saveScene(const char* path);
	
};

#endif // __SAVELOADMANAGER_H__
