#include "Globals.h"
#include "Application.h"
#include "ModuleSceneMain.h"
#include "ElementGameUI.h"
#include "UIButton.h"
#include "MathGeoLib/src/Math/MathFunc.h"
#include "MathGeoLib/src/Math/float3x4.h"
#include "Imgui/imgui.h"

UIButton::UIButton(GameObject* parent, int x, int y, int h, int w, bool isVisible) : ElementGameUI(parent, x, y, h, w, isVisible)
{
	type = BUTTON;
}

UIButton::~UIButton() {}

void UIButton::drawGUI()
{
	if (ImGui::CollapsingHeader("Button"))
	{
		ImGui::Checkbox("Visible", &visible);

		ImGui::Text("Position");
		ImGui::SameLine(78);
		ImGui::PushID("position");
		ImGui::NewLine();
		ImGui::DragInt("X", &rect.x, 1);
		ImGui::DragInt("Y", &rect.y, 1);
		ImGui::PopID();

		ImGui::Text("Size");
		ImGui::SameLine(70);
		ImGui::PushID("size");
		ImGui::NewLine();
		ImGui::DragInt("Height", &rect.h, 1);
		ImGui::DragInt("Width", &rect.w, 1);
		ImGui::PopID();
	}
}