#include "Globals.h"
#include "Application.h"
#include "ModuleSceneMain.h"
#include "ElementGameUI.h"
#include "UILabel.h"
#include "MathGeoLib/src/Math/MathFunc.h"
#include "MathGeoLib/src/Math/float3x4.h"
#include "Imgui/imgui.h"
#include "GameObject.h"
#include "ModuleType.h"

UILabel::UILabel(GameObject* parent, int x, int y, int h, int w, bool isVisible) : ElementGameUI(parent, x, y, h, w, isVisible)
{
	type = LABEL;
	text = "New Label";
	fontPath = "Assets/Fonts/Roboto-Regular.ttf";
	App->type->loadFont("Assets/Fonts/Roboto-Regular.ttf");
}

UILabel::~UILabel() {}

void UILabel::drawGUI()
{
	if (ImGui::CollapsingHeader("Label"))
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

		fillGUI();
	}
}

void UILabel::fillGUI()
{
	char aux1[64];
	strcpy_s(aux1, 64, text.c_str());

	ImGui::PushID("Text");
	if (ImGui::InputText("", aux1, 64))
	{
		text = aux1;
	}
	ImGui::PopID();

	char aux2[128];
	strcpy_s(aux2, 128, fontPath.c_str());
	ImGui::PushID("FontPath");
	if (ImGui::InputText("", aux2, 128))
	{
		fontPath = aux2;
	}
	ImGui::PopID();
}
