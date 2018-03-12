#ifndef __ELEMENTGAMEUI_H__
#define __ELEMENTGAMEUI_H__

#include "Module.h"
#include <string>

class ElementGameUI
{
public:
	ElementGameUI(GameObject* parent, int x, int y, int h, int w, bool isVisible = true);
	~ElementGameUI();
	virtual void drawGUI() = 0;
	void startGUI();
	void endGUI();


public:
	TypeElemeneGametUI type;
	GameObject* gameObject = nullptr;
	SDL_Rect rect;
	bool visible;
};

#endif // __ELEMENTGAMEUI_H__