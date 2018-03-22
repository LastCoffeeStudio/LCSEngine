#include "Globals.h"
#include "Application.h"
#include "ModuleSceneMain.h"
#include "ElementGameUI.h"
#include "UIButton.h"
#include "Imgui/imgui.h"
#include "GameObject.h"
#include "UIImage.h"
#include "UILabel.h"
#include "ElementFactory.h"
#include "ModuleInput.h"


UIButton::UIButton(GameObject* parent, int x, int y, int h, int w, bool isVisible) : ElementGameUI(parent, x, y, h, w, isVisible)
{
	type = BUTTON;
	static ElementFactory* factoryElements = ElementFactory::getInstance();
	label = (UILabel*)(factoryElements->getComponent(LABEL, nullptr, x, y, h, w, true));
	background = (UIImage*)(factoryElements->getComponent(IMAGE, nullptr, x, y, h, w, true));
	hover = (UIImage*)(factoryElements->getComponent(IMAGE, nullptr, x, y, h, w, true));
	pressed = (UIImage*)(factoryElements->getComponent(IMAGE, nullptr, x, y, h, w, true));
	disabled = (UIImage*)(factoryElements->getComponent(IMAGE, nullptr, x, y, h, w, true));
	activeImage = background;
}

UIButton::UIButton(GameObject* parent) : ElementGameUI(parent) {}

UIButton::~UIButton() {}

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

