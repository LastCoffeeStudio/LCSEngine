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

UIEditText::UIEditText(GameObject* parent, int x, int y, int h, int w, bool isVisible) : ElementGameUI(parent, x, y, h, w, isVisible)
{
	type = EDITTEXT;
	static ElementFactory* factoryElements = ElementFactory::getInstance();
	label = (UILabel*)(factoryElements->getComponent(LABEL, nullptr, x, y, h, w, true));
	background = (UIImage*)(factoryElements->getComponent(IMAGE, nullptr, x, y, h, w, true));
	selected = (UIImage*)(factoryElements->getComponent(IMAGE, nullptr, x, y, h, w, true));
}

UIEditText::~UIEditText() {}

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

		ImGui::InputText("Selected", &selected->textureName[0], IM_ARRAYSIZE(selected->textureName));
		if (ImGui::Button("Set hover") && selected->textureName != selected->texName)
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
}
