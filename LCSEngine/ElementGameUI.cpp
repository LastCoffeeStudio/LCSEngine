#include "Globals.h"
#include "Application.h"
#include "ElementGameUI.h"

ElementGameUI::ElementGameUI(TypeElemeneGametUI typeElement, int x, int y, int h, int w, bool isVisible)
{
	type = typeElement;
	visible = isVisible;
	rect.x = x;
	rect.y = y;
	rect.h = h;
	rect.w = w;
}

ElementGameUI::~ElementGameUI() {}