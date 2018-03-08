#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "ElementGameUI.h"

ElementGameUI::ElementGameUI(GameObject* parent, int x, int y, int h, int w, bool isVisible)
{
	gameObject = parent;
	visible = isVisible;
	rect.x = x;
	rect.y = y;
	rect.h = h;
	rect.w = w;
}

ElementGameUI::~ElementGameUI() {}