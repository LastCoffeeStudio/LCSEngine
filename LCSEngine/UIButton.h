#ifndef __UIBUTTON_H__
#define __UIBUTTON_H__

#include "ElementGameUI.h"

class UIButton : public ElementGameUI
{
public:
	UIButton(GameObject* parent, int x, int y, int h, int w, bool isVisible = true);
	~UIButton();

};

#endif //__UIBUTTON_H__