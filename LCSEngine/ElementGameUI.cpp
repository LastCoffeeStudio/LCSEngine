#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "ElementGameUI.h"
#include "Imgui/imgui.h"
#include "ModuleSceneMain.h"
#include "MathGeoLib/src/Math/MathFunc.h"
#include "MathGeoLib/src/Math/float3x4.h"
#include "ModuleInput.h"


ElementGameUI::ElementGameUI(GameObject* parent, int x, int y, int h, int w, bool isVisible)
{
	gameObject = parent;
    if (gameObject != nullptr)
    {
        UUIDparent = parent->UUID;
    }
	visible = isVisible;
	rect.x = x;
	rect.y = y;
	rect.h = h;
	rect.w = w;
	enable = true;
	UUIDelement = uuid();
}

ElementGameUI::ElementGameUI(GameObject * parent)
{
	gameObject = parent;
	if (gameObject != nullptr)
	{
		UUIDparent = parent->UUID;
	}
}

ElementGameUI::~ElementGameUI() {}

void ElementGameUI::startGUI()
{
}

void ElementGameUI::endGUI()
{
}

bool ElementGameUI::isHover()
{
	iPoint mousePosition = App->input->getMousePosition();
	if (mousePosition.x > rect.x && mousePosition.x < rect.x + rect.w)
	{
		if (mousePosition.y > rect.y && mousePosition.y < rect.y + rect.h)
		{
			return true;
		}
	}
	return false;
}

void ElementGameUI::load(nlohmann::json& conf)
{
	visible = conf.at("visible").get<bool>();
	enable = conf.at("enable").get<bool>();
	rect.x = conf.at("x").get<int>();
	rect.y = conf.at("y").get<int>();
	rect.h = conf.at("h").get<int>();
	rect.w = conf.at("w").get<int>();
}

void ElementGameUI::save(nlohmann::json& conf)
{
	conf["visible"] = visible;
	conf["enable"] = enable;
	conf["TypeElemeneGametUI"] = type;
	conf["x"] = rect.x;
	conf["y"] = rect.y;
	conf["w"] = rect.w;
	conf["h"] = rect.h;

}