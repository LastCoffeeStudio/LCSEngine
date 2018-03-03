#include "ModuleAnimation.h"

ModuleAnimation::ModuleAnimation() {}

ModuleAnimation::~ModuleAnimation() {}

bool ModuleAnimation::cleanUp()
{
	return true;
}

bool ModuleAnimation::load(const char* name, const char* path)
{
	bool success = false;
	return success;
}

update_status ModuleAnimation::update(float deltaTime)
{
	return UPDATE_CONTINUE;
}

unsigned int ModuleAnimation::play(const char* name)
{
	unsigned int id = -1;
	return id;
}

void ModuleAnimation::stop(unsigned int id) {}

bool ModuleAnimation::getTransform(unsigned int id, const char* frameName, float3& position, Quat& rotation)
{
	bool success = false;
	return success;
}