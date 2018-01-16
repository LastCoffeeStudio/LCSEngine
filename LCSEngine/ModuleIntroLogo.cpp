#include "Globals.h"
#include "Application.h"
#include "ModuleIntroLogo.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleJson.h"


#include <iostream>
using namespace std;

ModuleIntroLogo::ModuleIntroLogo()
{
}


ModuleIntroLogo::~ModuleIntroLogo()
{
}

bool ModuleIntroLogo::Start()
{
	background = App->textures->Load("Assets/Images/logo_lcs.png");

	return true;
}

update_status ModuleIntroLogo::Update(const float deltaTime)
{
	App->renderer->Blit(background, 0, 0, NULL);
	return UPDATE_CONTINUE;
}
