#ifndef __UILABEL_H__
#define __UILABEL_H__

#include "ElementGameUI.h"

class UILabel : public ElementGameUI
{
public:
	UILabel(GameObject* parent, int x, int y, int h, int w, bool isVisible = true);
	~UILabel();
	void drawGUI() override;
	void fillGUI();

public:
	std::string text;

};

#endif //__UILABEL_H__