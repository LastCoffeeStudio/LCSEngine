#ifndef __MODULEINPUT_H__
#define __MODULEINPUT_H__

#include "Module.h"
#include "Point.h"
#include "SDL/include/SDL_scancode.h"

#define NUM_MOUSE_BUTTONS 5

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{

public:

	ModuleInput();

	// Destructor
	virtual ~ModuleInput();

	// Called before render is available
	bool init();

	// Called before the first frame
	bool start();

	// Called each loop iteration
	update_status preUpdate(float deltaTime);

	// Called before quitting
	bool cleanUp();

	// Check key states (includes mouse and joy buttons)
	KeyState getKey(int id) const
	{
		return keyboard[id];
	}

	KeyState getMouseButtonDown(int id) const
	{
		return mouse_buttons[id - 1];
	}

	// Check for window events last frame
	bool getWindowEvent(EventWindow code) const;

	// Get mouse / axis position
	const iPoint& getMouseMotion() const;
	const iPoint& getMousePosition() const;
	int mouse_wheel;

	//Lua methods
public:
	static bool getButtonDown();

public:
	char keyPresed = '\0';

private:
	bool		windowEvents[WE_COUNT];
	KeyState*	keyboard;
	KeyState	mouse_buttons[NUM_MOUSE_BUTTONS];
	iPoint mouse_motion;
	iPoint mouse;
};

#endif // __MODULEINPUT_H__