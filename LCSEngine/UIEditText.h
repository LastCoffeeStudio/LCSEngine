#ifndef __UIEDITTEXT_H__
#define __UIEDITTEXT_H__

#include "ElementGameUI.h"

class UILabel;
class UIImage;

class UIEditText : public ElementGameUI
{
public:
	UIEditText(GameObject* parent, int x, int y, int h, int w, bool isVisible = true);
	UIEditText(GameObject* parent);
	~UIEditText();
	void drawGUI() override;
	void update();
	void load(nlohmann::json& conf);
	void save(nlohmann::json& conf);

private: 
	void init(int x, int y, int h, int w, bool isVisible);

public:
	UILabel* label = nullptr;
	UIImage* background = nullptr;
	UIImage* selected = nullptr;
	bool focus = false;

private:
	std::string text = "";
	int paddingX = 0;
	int paddingY = 0;
	SDL_Rect originRect;
};

#endif //__UIEDITTEXT_H__