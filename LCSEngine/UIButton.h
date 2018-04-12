#ifndef __UIBUTTON_H__
#define __UIBUTTON_H__

#include "ElementGameUI.h"

class UILabel;
class UIImage;

class UIButton : public ElementGameUI
{
public:
	UIButton(GameObject* parent, int x, int y, int h, int w, bool isVisible = true);
	UIButton(GameObject* parent);
	~UIButton();
	void drawGUI() override;
	void deleteElementButton();
	void update();
	void load(nlohmann::json& conf);
	void save(nlohmann::json& conf);

public:
	UIImage* activeImage = nullptr;
	UILabel* label = nullptr;

private:
	void updateState();
	void init(int x, int y, int h, int w, bool isVisible);

private:
	UIImage* background = nullptr;
	UIImage* hover = nullptr;
	UIImage* pressed = nullptr;
	UIImage* disabled = nullptr;
	int paddingX = 0;
	int paddingY = 0;
};

#endif //__UIBUTTON_H__