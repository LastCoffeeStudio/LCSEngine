#include "Globals.h"
#include "Application.h"
#include "ModuleIntro.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleJson.h"

#include <iostream>
using namespace std;

ModuleIntro::ModuleIntro() {}


ModuleIntro::~ModuleIntro() {}

bool ModuleIntro::Start()
{

	return true;
}

update_status ModuleIntro::Update(const float deltaTime)
{
	
	return UPDATE_CONTINUE;
}
