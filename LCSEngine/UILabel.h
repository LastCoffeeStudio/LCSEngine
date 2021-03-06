#ifndef __UILABEL_H__
#define __UILABEL_H__

#include "ElementGameUI.h"

struct FontData;
class UIImage;

class UILabel : public ElementGameUI
{
public:
	UILabel(GameObject* parent, int x, int y, int h, int w, bool isVisible = true);
	UILabel(GameObject* parent);
	~UILabel();
	void drawGUI() override;
	void fillGUI();
	void fillBufferData();
	void update();
	void load(nlohmann::json& conf);
	void save(nlohmann::json& conf);

private: 
	void init(int x, int y, int h, int w, bool isVisible);
public:
	std::string text;
	std::string fontPath;
	int fontSize;
	float textColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	SDL_Color color = {255, 255, 255, 255};
	GLuint idTexture;
	FontData* fontData = nullptr;
	UIImage* textImage = nullptr;
};

#endif //__UILABEL_H__