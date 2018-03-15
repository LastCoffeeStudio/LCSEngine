#ifndef __UILABEL_H__
#define __UILABEL_H__

#include "ElementGameUI.h"

struct FontData;
class UIImage;

class UILabel : public ElementGameUI
{
public:
	UILabel(GameObject* parent, int x, int y, int h, int w, bool isVisible = true);
	~UILabel();
	void drawGUI() override;
	void fillGUI();
	void update();

public:
	std::string text;
	std::string fontPath;
	FontData* fontData = nullptr;
	UIImage* textImage = nullptr;
};

#endif //__UILABEL_H__