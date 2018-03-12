#ifndef __UIEDITTEXT_H__
#define __UIEDITTEXT_H__

#include "ElementGameUI.h"

class UIEditText : public ElementGameUI
{
public:
	UIEditText(GameObject* parent, int x, int y, int h, int w, bool isVisible = true);
	~UIEditText();
	void drawGUI() override;

};

#endif //__UIEDITTEXT_H__