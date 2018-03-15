#ifndef __UIBUTTON_H__
#define __UIBUTTON_H__

#include "ElementGameUI.h"

class UILabel;
class UIImage;

class UIButton : public ElementGameUI
{
public:
	UIButton(GameObject* parent, int x, int y, int h, int w, bool isVisible = true);
	~UIButton();
	void drawGUI() override;
	void deleteElementButton();
	void update();

public:
	UIImage* activeImage = nullptr;
	UILabel* label = nullptr;

private:
	void updateState();
	bool isHover();

private:
	UIImage* background = nullptr;
	UIImage* hover = nullptr;
	UIImage* pressed = nullptr;
	UIImage* disabled = nullptr;
};

#endif //__UIBUTTON_H__