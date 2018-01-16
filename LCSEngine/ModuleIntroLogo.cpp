#include "Globals.h"
#include "Application.h"
#include "ModuleIntroLogo.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleJson.h"


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

update_status ModuleIntroLogo::Update()
{
	App->renderer->Blit(background, 0, 0, NULL);

	return UPDATE_CONTINUE;
}
