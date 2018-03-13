#include "Globals.h"
#include "Application.h"
#include "ModuleSceneMain.h"
#include "ElementGameUI.h"
#include "UIButton.h"
#include "MathGeoLib/src/Math/MathFunc.h"
#include "MathGeoLib/src/Math/float3x4.h"
#include "Imgui/imgui.h"
#include "GameObject.h"
#include "UIImage.h"
#include "UILabel.h"
#include "ElementFactory.h"

UIButton::UIButton(GameObject* parent, int x, int y, int h, int w, bool isVisible) : ElementGameUI(parent, x, y, h, w, isVisible)
{
	type = BUTTON;
	static ElementFactory* factoryElements = ElementFactory::getInstance();
	label = (UILabel*)(factoryElements->getComponent(LABEL, nullptr, x, y, h, w, true));
	background = (UIImage*)(factoryElements->getComponent(IMAGE, nullptr, x, y, h, w, true));
	hover = (UIImage*)(factoryElements->getComponent(IMAGE, nullptr, x, y, h, w, true));
	pressed = (UIImage*)(factoryElements->getComponent(IMAGE, nullptr, x, y, h, w, true));
	dissabled = (UIImage*)(factoryElements->getComponent(IMAGE, nullptr, x, y, h, w, true));
}

UIButton::~UIButton() {}

void UIButton::drawGUI()
{
	
	if (ImGui::CollapsingHeader("Button"))
	{
		ImGui::Checkbox("Visible", &visible);

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
		ImGui::Text("Background:");
		background->fillGUI();
		ImGui::Text("Hover:");
		hover->fillGUI();
		ImGui::Text("Pressed:");
		pressed->fillGUI();
		ImGui::Text("Dissabled:");
		dissabled->fillGUI();
	}
}

void UIButton::deleteElementButton()
{
	RELEASE(label);
	RELEASE(background);
	RELEASE(hover);
	RELEASE(pressed);
	RELEASE(dissabled);
}