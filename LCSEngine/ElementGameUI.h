#ifndef __ELEMENTGAMEUI_H__
#define __ELEMENTGAMEUI_H__

#include "Module.h"
#include <string>

class ElementGameUI
{
public:
	ElementGameUI(TypeElemeneGametUI typeElement, int x, int y, int h, int w, bool isVisible = true);
	~ElementGameUI();

public:
	TypeElemeneGametUI type;
	SDL_Rect rect;
	bool visible;
};

#endif // __ELEMENTGAMEUI_H__