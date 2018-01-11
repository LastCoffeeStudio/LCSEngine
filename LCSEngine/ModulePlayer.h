#ifndef __MODULEPLAYER_H__
#define __MODULEPLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "Point.h"

struct SDL_Texture;

enum playerState {
	PLAYER_IDLE,
	PLAYER_MOV_BACK,
	PLAYER_MOV_FORW
};

class ModulePlayer : public Module
{
public:
	ModulePlayer(bool start_enabled = true);
	~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

public:

	SDL_Texture* graphics = nullptr;
	Animation idle;
	Animation backward;
	Animation walkForward;
	iPoint position;
	playerState playerState;
};

#endif // __MODULEPLAYER_H__