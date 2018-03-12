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

private:
	UILabel* label = nullptr;
	UIImage* background = nullptr;
};

#endif //__UIBUTTON_H__