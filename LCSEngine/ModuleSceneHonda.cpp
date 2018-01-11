#include "Globals.h"
#include "Application.h"
#include "ModuleSceneHonda.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "SDL/include/SDL.h"
#include <time.h>

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModuleSceneHonda::ModuleSceneHonda(bool start_enabled) : Module(start_enabled)
{
	// ground
	ground.x = 8;
	ground.y = 376;
	ground.w = 848;
	ground.h = 64;

	// top
	top.x = 91;
	top.y = 8;
	top.w = 765;
	top.h = 48;

	// foreground
	foreground.x = 164;
	foreground.y = 66;
	foreground.w = 336;
	foreground.h = 51;


	// Background
	background.x = 120;
	background.y = 128;
	background.w = 671;
	background.h = 199;

	// sun
	sun.frames.push_back({ 224, 64, 224, 104 });
	sun.frames.push_back({ 456, 64, 224, 104 });
	sun.speed = 0.01f;

	// man
	man.frames.push_back({ 688, 64, 96, 104 });
	man.frames.push_back({ 792, 64, 64, 104 });
	man.speed = 0.01f;

	// water
	water.frames.push_back({ 8, 448, 284, 8 });
	water.frames.push_back({ 296, 448, 284, 12 });
	water.frames.push_back({ 588, 448, 284, 18 });
	water.speed = 0.05f;
}

ModuleSceneHonda::~ModuleSceneHonda()
{}

// Load assets
bool ModuleSceneHonda::Start()
{
	LOG("Loading honda scene");

	graphics = App->textures->Load("honda_stage.png");
	graphicsBackground = App->textures->Load("honda_stage2.png");

	if (App->player->IsEnabled() == false) {
		if (App->player->Enable() == false) {
			LOG("Couldn't load player");
		}
	}

	/*hondaMusicId = App->audio->LoadFx("honda.ogg");
	if (hondaMusicId == 0) {
	LOG("Couldn't load background audio");
	}
	if (App->audio->PlayFx(hondaMusicId) == false) {
	LOG("Couldn't play background audio");
	}*/
	if (App->audio->PlayMusic("honda.ogg", 5.0f) == false) {
		LOG("Couldn't play honda music");
	}

	return true;
}

// UnLoad assets
bool ModuleSceneHonda::CleanUp()
{
	LOG("Unloading honda scene");

	App->textures->Unload(graphicsBackground);
	App->textures->Unload(graphics);
	App->player->Disable();

	return true;
}

// Update: draw background
update_status ModuleSceneHonda::Update()
{
	// Draw everything --------------------------------------
	App->renderer->Blit(graphics, -80, 170, &ground, 1.f);

	App->renderer->Blit(graphicsBackground, -20, 0, &background, 1.f);
	App->renderer->Blit(graphics, 172, 56, &(sun.GetCurrentFrame()), 1.f);
	App->renderer->BlitWithPivotUpRight(graphics, 492, 56, &(man.GetCurrentFrame()), 1.f);

	App->renderer->Blit(graphics, -70, 0, &top, 1.f);

	App->renderer->Blit(graphicsBackground, 160, 135, &foreground, 1.f);
	App->renderer->BlitWithPivotUpRight(graphics, 472, 147, &(water.GetCurrentFrame()), 1.f);

	//App->renderer->Blit(graphics, 560, 8, &(sun.GetCurrentFrame()), 1.3f);
	//App->renderer->Blit(graphics, 0, 0, &foreground, 1.5f);
	//App->renderer->Blit(graphics, 193, 105, &(man.GetCurrentFrame()), 1.5f);


	return UPDATE_CONTINUE;
}