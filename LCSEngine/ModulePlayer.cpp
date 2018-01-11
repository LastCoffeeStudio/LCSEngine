#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA
ModulePlayer::ModulePlayer(bool start_enabled) : Module(start_enabled)
{
	position.x = 100;
	position.y = 216;
	playerState = PLAYER_IDLE;

	// idle animation (arcade sprite sheet)
	idle.frames.push_back({ 7, 14, 60, 90 });
	idle.frames.push_back({ 95, 15, 60, 89 });
	idle.frames.push_back({ 184, 14, 60, 90 });
	idle.frames.push_back({ 276, 11, 60, 93 });
	idle.frames.push_back({ 366, 12, 60, 92 });
	idle.speed = 0.2f;

	// walk backward animation (arcade sprite sheet)
	backward.frames.push_back({ 542, 131, 61, 87 });
	backward.frames.push_back({ 628, 129, 59, 90 });
	backward.frames.push_back({ 713, 128, 57, 90 });
	backward.frames.push_back({ 797, 127, 57, 90 });
	backward.frames.push_back({ 883, 128, 58, 91 });
	backward.frames.push_back({ 974, 129, 57, 89 });
	backward.speed = 0.1f;

	// TODO 8: setup the walk forward animation from ryu4.png
	walkForward.frames.push_back({ 9, 136, 53, 85 });
	walkForward.frames.push_back({ 78, 131, 62, 90 });
	walkForward.frames.push_back({ 162, 128, 65, 94 });
	walkForward.frames.push_back({ 259, 128, 62, 94 });
	walkForward.frames.push_back({ 352, 128, 55, 94 });
	walkForward.frames.push_back({ 432, 131, 50, 91 });
	walkForward.speed = 0.1f;
}

ModulePlayer::~ModulePlayer()
{
	// Homework : check for memory leaks
}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	graphics = App->textures->Load("ryu4.png"); // arcade version

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);

	return true;
}

// Update
update_status ModulePlayer::Update()
{
	// TODO 9: Draw the player with its animation
	// make sure to detect player movement and change its
	// position while cycling the animation(check Animation.h)
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		position.x -= 1;
		playerState = PLAYER_MOV_BACK;
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		position.x += 1;
		playerState = PLAYER_MOV_FORW;
	}
	else {
		playerState = PLAYER_IDLE;
	}

	if (playerState == PLAYER_MOV_FORW)
		//App->renderer->Blit(graphics, position.x, position.y, &(idle.GetCurrentFrame()), 1.0f);
		App->renderer->BlitWithPivotDownCenter(graphics, position.x, position.y, &(walkForward.GetCurrentFrame()), 1.0f);
	else if (playerState == PLAYER_MOV_BACK)
		App->renderer->BlitWithPivotDownCenter(graphics, position.x, position.y, &(backward.GetCurrentFrame()), 1.0f);
	else if (playerState == PLAYER_IDLE)
		//App->renderer->Blit(graphics, position.x, position.y, &(idle.GetCurrentFrame()), 1.0f);
		App->renderer->BlitWithPivotDownCenter(graphics, position.x, position.y, &(idle.GetCurrentFrame()), 1.0f);


	return UPDATE_CONTINUE;
}

