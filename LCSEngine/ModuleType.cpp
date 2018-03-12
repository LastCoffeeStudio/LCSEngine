#include "ModuleType.h"


ModuleType::ModuleType(){}

ModuleType::~ModuleType(){}

bool ModuleType::init()
{
	FT_Error error = FT_Init_FreeType(&library);
	if (error)
	{
		LOG("Cannot load Freetype library");
	}

	return true;
}

update_status ModuleType::update(float deltaTime)
{
	return UPDATE_CONTINUE;
}
