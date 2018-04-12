#ifndef __ELEMENTGAMEUI_H__
#define __ELEMENTGAMEUI_H__

#include "Module.h"
#include <json.hpp>
#include <string>

class ElementGameUI
{
public:
	ElementGameUI(GameObject* parent, int x, int y, int h, int w, bool isVisible = true);
	ElementGameUI(GameObject* parent);
	~ElementGameUI();
	virtual void drawGUI() = 0;
	void startGUI();
	void endGUI();
	bool isHover();
	virtual void load(nlohmann::json& conf);
	virtual void save(nlohmann::json& conf);

public:
	TypeElemeneGametUI type;
	GameObject* gameObject = nullptr;
	SDL_Rect rect;
	bool visible;
	bool enable;
    unsigned int UUIDparent = NULL;
	unsigned int UUIDelement = -1;
};

#endif // __ELEMENTGAMEUI_H__