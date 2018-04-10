#include "ModuleTime.h"



ModuleTime::ModuleTime()
{
}


ModuleTime::~ModuleTime()
{
}

bool ModuleTime::init()
{
	actualRealTime = 0;
	actualGameTime = 0;
	return  true;
}

update_status ModuleTime::update(float deltaTime)
{
	actualRealTime += deltaTime;

	if(gameTimeActive)
	{
		actualGameTime += deltaTime;
	}
	return  UPDATE_CONTINUE;
}

void ModuleTime::startGameTime()
{
	gameTimeActive = true;
}

void ModuleTime::pauseGameTime()
{
	gameTimeActive = false;
}

void ModuleTime::stopGameTime()
{
	gameTimeActive = false;
	actualGameTime = 0;
}

float ModuleTime::getRealTime()
{
	return actualRealTime;
}

float ModuleTime::getGameTime()
{
	return actualGameTime;
}
