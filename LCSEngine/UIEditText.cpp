#include "Globals.h"
#include "Application.h"
#include "ModuleSceneMain.h"
#include "ElementGameUI.h"
#include "UIEditText.h"
#include "MathGeoLib/src/Math/MathFunc.h"
#include "MathGeoLib/src/Math/float3x4.h"
#include "Imgui/imgui.h"
#include "GameObject.h"
#include "ElementFactory.h"
#include "UIImage.h"
#include "UILabel.h"
#include "ModuleInput.h"
#include <string>
#include "ModuleType.h"

UIEditText::UIEditText(GameObject* parent, int x, int y, int h, int w, bool isVisible) : ElementGameUI(parent, x, y, h, w, isVisible)
{
	type = EDITTEXT;
	init(x,y,h,w,isVisible);
}

UIEditText::UIEditText(GameObject* parent) : ElementGameUI(parent) {}

UIEditText::~UIEditText() {}

void UIEditText::init(int x, int y, int h, int w, bool isVisible)
{
	static ElementFactory* factoryElements = ElementFactory::getInstance();
	label = (UILabel*)(factoryElements->getComponent(LABEL, nullptr, x, y, h, w, true));
	background = (UIImage*)(factoryElements->getComponent(IMAGE, nullptr, x, y, h, w, true));
	selected = (UIImage*)(factoryElements->getComponent(IMAGE, nullptr, x, y, h, w, true));
	text = label->text;
}

void UIEditText::drawGUI()
{
	if (ImGui::CollapsingHeader("EditText"))
	{
		ImGui::Checkbox("Visible", &visible);
		ImGui::Checkbox("Enable", &enable);

		if (ImGui::Button("Delete Component"))
		{
			App->sceneMain->garbageCollectorElements.push_back(this);
		}

		ImGui::Text("Position");
		ImGui::PushID("position");
		if (ImGui::DragInt("X", &rect.x, 1))
		{
			label->rect.x = rect.x + paddingX;
			label->update();
			label->fillBufferData();
		}
		if (ImGui::DragInt("Y", &rect.y, 1))
		{
			label->rect.y = rect.y + paddingY;
			label->update();
			label->fillBufferData();
		}
		ImGui::PopID();

		ImGui::Text("Padding");
		ImGui::PushID("padding");
		if (ImGui::DragInt("X", &paddingX, 1))
		{
			rect.w = label->rect.w + paddingX * 2;
			label->rect.x = rect.x + paddingX;
			label->update();
			label->fillBufferData();
		}
		if (ImGui::DragInt("Y", &paddingY, 1))
		{
			rect.h = label->rect.h + paddingY * 2;
			label->rect.y = rect.y + paddingY;
			label->update();
			label->fillBufferData();
		}
		ImGui::PopID();

		ImGui::Text("Size");
		ImGui::PushID("size");
		ImGui::DragInt("Height", &rect.h, 1);
		ImGui::DragInt("Width", &rect.w, 1);
		ImGui::PopID();

		label->fillGUI();

		ImGui::InputText("Background", &background->textureName[0], IM_ARRAYSIZE(background->textureName));
		if (ImGui::Button("Set background") && background->textureName != background->texName)
		{
			background->textureChanged = true;
		}

		ImGui::InputText("Selected", &selected->textureName[0], IM_ARRAYSIZE(selected->textureName));
		if (ImGui::Button("Set selected") && selected->textureName != selected->texName)
		{
			selected->textureChanged = true;
		}
	}
}

void UIEditText::update()
{
	background->rect = rect;
	selected->rect = rect;
	background->updateCoords();
	selected->updateCoords();

	KeyState click = App->input->getMouseButtonDown(1);
	bool mouseHoverButton = isHover();
	if (mouseHoverButton && click == KEY_DOWN)
	{
		focus = true;
		App->input->keyPresed = '\0';
	}
	else if (!mouseHoverButton && click == KEY_DOWN)
	{
		focus = false;
	}

	if (focus && App->input->keyPresed != '\0')
	{
		if (App->input->keyPresed != 8)
		{
			char aux = App->input->keyPresed;
			text += aux;
		}
		else
		{
			if(text.size() > 0)
			{
				text.pop_back();
			}
		}
		label->text = text;
		App->input->keyPresed = '\0';
		App->type->loadFont(label->fontPath.c_str(), label->fontSize);
		label->fontData = App->type->renderFont(text.c_str(), label->fontPath.c_str(), label->color);
		label->fillBufferData();
	}
}

void UIEditText::load(nlohmann::json& conf)
{
	ElementGameUI::load(conf);
	type = EDITTEXT;
	init(rect.x, rect.y, rect.h, rect.w, visible);

	background->hasTexture = conf.at("background_hasTexture").get<bool>();
	background->texName = conf.at("background_texName").get<std::string>().c_str();
	strcpy_s(background->textureName, 128, background->texName.c_str());
	background->texName = "";
	background->textureChanged = background->hasTexture;
	background->hasTexture = false;

	selected->hasTexture = conf.at("selected_hasTexture").get<bool>();
	selected->texName = conf.at("selected_texName").get<std::string>().c_str();
	strcpy_s(selected->textureName, 128, selected->texName.c_str());
	selected->texName = "";
	selected->textureChanged = selected->hasTexture;
	selected->hasTexture = false;

	label->text = conf.at("text").get<std::string>();
	label->fontPath = conf.at("fontPath").get<std::string>();
	label->fontSize = conf.at("fontSize").get<int>();

	App->type->loadFont(label->fontPath.c_str(), label->fontSize);
	label->fontData = App->type->renderFont(label->text.c_str(), label->fontPath.c_str(), label->color);

	paddingX = conf.at("paddingX").get<int>();
	paddingY = conf.at("paddingY").get<int>();

	label->update();
	label->fillBufferData();
}

void UIEditText::save(nlohmann::json& conf)
{
	ElementGameUI::save(conf);
	conf["background_hasTexture"] = background->hasTexture;
	conf["background_texName"] = background->texName;

	conf["selected_hasTexture"] = selected->hasTexture;
	conf["selected_texName"] = selected->texName;

	conf["text"] = label->text;
	conf["fontPath"] = label->fontPath;
	conf["fontSize"] = label->fontSize;

	conf["paddingX"] = paddingX;
	conf["paddingY"] = paddingY;
}