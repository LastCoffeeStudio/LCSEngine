#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "ModuleRender.h"
#include "ModuleGUI.h"
#include "SDL/include/SDL.h"
#include "Brofiler.h"
#include <string>

#define MAX_KEYS 300

ModuleInput::ModuleInput() : Module(), mouse({ 0, 0 }), mouse_motion({ 0,0 })
{
	keyboard = new KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	RELEASE_ARRAY(keyboard);
}

// Called before render is available
bool ModuleInput::init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool ModuleInput::start()
{
	return true;
}

// Called each loop iteration
update_status ModuleInput::preUpdate(float deltaTime)
{
	BROFILER_CATEGORY("ModuleInput", Profiler::Color::Orchid)
	static SDL_Event event;

	mouse_motion = { 0, 0 };
	memset(windowEvents, false, WE_COUNT * sizeof(bool));

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
			{
				keyboard[i] = KEY_DOWN;
			}
			else
			{
				keyboard[i] = KEY_REPEAT;
			}
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
			{
				keyboard[i] = KEY_UP;
			}
			else
			{
				keyboard[i] = KEY_IDLE;
			}
		}
	}

	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouse_buttons[i] == KEY_DOWN)
		{
			mouse_buttons[i] = KEY_REPEAT;
		}

		if (mouse_buttons[i] == KEY_UP)
		{
			mouse_buttons[i] = KEY_IDLE;
		}
	}
	mouse_wheel = 0;
	while (SDL_PollEvent(&event) != 0)
	{
		ImGui_ImplSdlGL3_ProcessEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			windowEvents[WE_QUIT] = true;
			break;

		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
				//case SDL_WINDOWEVENT_LEAVE:
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				windowEvents[WE_HIDE] = true;
				break;

				//case SDL_WINDOWEVENT_ENTER:
			case SDL_WINDOWEVENT_RESIZED:
					App->camera->updatedWindowSize(event.window.data1, event.window.data2);
					App->renderer->updatedWindowSize(event.window.data1, event.window.data2);
				break;
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				windowEvents[WE_SHOW] = true;
				break;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			mouse_buttons[event.button.button - 1] = KEY_DOWN;
			break;

		case SDL_MOUSEBUTTONUP:
			mouse_buttons[event.button.button - 1] = KEY_UP;
			break;

		case SDL_MOUSEMOTION:
			mouse_motion.x = event.motion.xrel / SCREEN_SIZE;
			mouse_motion.y = event.motion.yrel / SCREEN_SIZE;
			mouse.x = event.motion.x / SCREEN_SIZE;
			mouse.y = event.motion.y / SCREEN_SIZE;
			break;
		case SDL_MOUSEWHEEL:
			mouse_wheel = event.wheel.y;
		}
	}

	if (getWindowEvent(EventWindow::WE_QUIT) == true || getKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		return UPDATE_STOP;
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::cleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

// ---------
bool ModuleInput::getWindowEvent(EventWindow ev) const
{
	return windowEvents[ev];
}

const iPoint& ModuleInput::getMousePosition() const
{
	return mouse;
}

const iPoint& ModuleInput::getMouseMotion() const
{
	return mouse_motion;
}

