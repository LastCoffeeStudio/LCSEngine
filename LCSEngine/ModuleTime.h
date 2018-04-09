#ifndef __MODULETIME_H__
#define __MODULETIME_H__

#include "Module.h"

class ModuleTime : public Module
{
public:
	ModuleTime();
	~ModuleTime();

	bool init() override;

	update_status update(float deltaTime) override;

	void startGameTime();
	void pauseGameTime();
	void stopGameTime();
	float getRealTime();
	float getGameTime();

private:
	float actualRealTime = 0;
	float actualGameTime = 0;
	
	bool gameTimeActive = false;

};

#endif // __MODULETIME_H__