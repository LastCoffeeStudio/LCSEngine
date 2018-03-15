#ifndef __UIEDITTEXT_H__
#define __UIEDITTEXT_H__

#include "ElementGameUI.h"

class UILabel;
class UIImage;

class UIEditText : public ElementGameUI
{
public:
	UIEditText(GameObject* parent, int x, int y, int h, int w, bool isVisible = true);
	~UIEditText();
	void drawGUI() override;
	void update();

public:
	UILabel* label = nullptr;
	UIImage* background = nullptr;
	UIImage* selected = nullptr;
	bool focus = false;


};

#endif //__UIEDITTEXT_H__