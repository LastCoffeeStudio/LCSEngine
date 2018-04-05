#include "Globals.h"
#include "Application.h"
#include "ModuleSceneMain.h"
#include "ElementGameUI.h"
#include "UIButton.h"
#include "Imgui/imgui.h"
#include "GameObject.h"
#include "UIImage.h"
#include "UILabel.h"
#include "ModuleType.h"
#include "ElementFactory.h"
#include "ModuleInput.h"


UIButton::UIButton(GameObject* parent, int x, int y, int h, int w, bool isVisible) : ElementGameUI(parent, x, y, h, w, isVisible)
{
	type = BUTTON;
	init(x,y,h,w,isVisible);
}

UIButton::UIButton(GameObject* parent) : ElementGameUI(parent) {}

UIButton::~UIButton() {}

void UIButton::init(int x, int y, int h, int w, bool isVisible)
{
	static ElementFactory* factoryElements = ElementFactory::getInstance();
	label = (UILabel*)(factoryElements->getComponent(LABEL, nullptr, x, y, h, w, true));
	background = (UIImage*)(factoryElements->getComponent(IMAGE, nullptr, x, y, h, w, true));
	hover = (UIImage*)(factoryElements->getComponent(IMAGE, nullptr, x, y, h, w, true));
	pressed = (UIImage*)(factoryElements->getComponent(IMAGE, nullptr, x, y, h, w, true));
	disabled = (UIImage*)(factoryElements->getComponent(IMAGE, nullptr, x, y, h, w, true));
	activeImage = background;
}

void UIButton::drawGUI()
{
	
	if (ImGui::CollapsingHeader("Button"))
	{
		ImGui::Checkbox("Visible", &visible);
		ImGui::Checkbox("Enable", &enable);

		if (ImGui::Button("Delete Component"))
		{
			App->sceneMain->garbageCollectorElements.push_back(this);
		}

		ImGui::Text("Position");
		ImGui::PushID("position");
		ImGui::DragInt("X", &rect.x, 1);
		ImGui::DragInt("Y", &rect.y, 1);
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

		ImGui::InputText("Hover", &hover->textureName[0], IM_ARRAYSIZE(hover->textureName));
		if (ImGui::Button("Set hover") && hover->textureName != hover->texName)
		{
			hover->textureChanged = true;
		}

		ImGui::InputText("Click", &pressed->textureName[0], IM_ARRAYSIZE(pressed->textureName));
		if (ImGui::Button("Set pressed") && pressed->textureName != pressed->texName)
		{
			pressed->textureChanged = true;
		}

		ImGui::InputText("Disabled", &disabled->textureName[0], IM_ARRAYSIZE(disabled->textureName));
		if (ImGui::Button("Set disabled") && disabled->textureName != disabled->texName)
		{
			disabled->textureChanged = true;
		}
	}
}

void UIButton::deleteElementButton()
{
	RELEASE(label);
	RELEASE(background);
	RELEASE(hover);
	RELEASE(pressed);
	RELEASE(disabled);
}

void UIButton::update()
{
	background->rect = rect;
	hover->rect = rect;
	pressed->rect = rect;
	disabled->rect = rect;

	label->rect = rect;
	background->updateCoords();
	hover->updateCoords();
	pressed->updateCoords();
	disabled->updateCoords();

	label->update();

	updateState();
}

void UIButton::updateState()
{
	KeyState click = App->input->getMouseButtonDown(1);
	bool mouseHoverButton = isHover();
	if (disabled->textureName[0] != '\0' && !enable)
	{
		activeImage = disabled;
	}
	else if (pressed->textureName[0] != '\0' && ((click == KEY_DOWN && mouseHoverButton) || (click == KEY_REPEAT && activeImage == pressed && mouseHoverButton)))
	{
		activeImage = pressed;
	}
	else if (mouseHoverButton && hover->textureName[0] != '\0')
	{
		activeImage = hover;
	}
	else
	{
		activeImage = background;
	}
}

void UIButton::load(nlohmann::json& conf)
{
	ElementGameUI::load(conf);
	type = BUTTON;
	init(rect.x, rect.y, rect.h, rect.w, visible);
	 
	background->hasTexture = conf.at("background_hasTexture").get<bool>();
	background->texName = conf.at("background_texName").get<std::string>().c_str();
	strcpy_s(background->textureName, 128, background->texName.c_str());
	background->texName = "";
	background->textureChanged = background->hasTexture;
	background->hasTexture = false;

	hover->hasTexture = conf.at("hover_hasTexture").get<bool>();
	hover->texName = conf.at("hover_texName").get<std::string>().c_str();
	strcpy_s(hover->textureName, 128, hover->texName.c_str());
	hover->texName = "";
	hover->textureChanged = hover->hasTexture;
	hover->hasTexture = false;

	pressed->hasTexture = conf.at("pressed_hasTexture").get<bool>();
	pressed->texName = conf.at("pressed_texName").get<std::string>().c_str();
	strcpy_s(pressed->textureName, 128, pressed->texName.c_str());
	pressed->texName = "";
	pressed->textureChanged = pressed->hasTexture;
	pressed->hasTexture = false;

	disabled->hasTexture = conf.at("disabled_hasTexture").get<bool>();
	disabled->texName = conf.at("disabled_texName").get<std::string>().c_str();
	strcpy_s(disabled->textureName, 128, disabled->texName.c_str());
	disabled->texName = "";
	disabled->textureChanged = disabled->hasTexture;
	disabled->hasTexture = false;

	activeImage = background;

	label->text = conf.at("text").get<std::string>();
	label->fontPath = conf.at("fontPath").get<std::string>();
	label->fontSize = conf.at("fontSize").get<int>();
	App->type->loadFont(label->fontPath.c_str(), label->fontSize);
	label->fontData = App->type->renderFont(label->text.c_str(), label->fontPath.c_str(), label->color);
	label->update();
	label->fillBufferData();
}

void UIButton::save(nlohmann::json& conf)
{
	ElementGameUI::save(conf);
	conf["background_hasTexture"] = background->hasTexture;
	conf["background_texName"] = background->texName;

	conf["hover_hasTexture"] = hover->hasTexture;
	conf["hover_texName"] = hover->texName;

	conf["pressed_hasTexture"] = pressed->hasTexture;
	conf["pressed_texName"] = pressed->texName;

	conf["disabled_hasTexture"] = disabled->hasTexture;
	conf["disabled_texName"] = disabled->texName;

	conf["text"] = label->text;
	conf["fontPath"] = label->fontPath;
	conf["fontSize"] = label->fontSize;
}

