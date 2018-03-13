#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "ElementGameUI.h"
#include "Imgui/imgui.h"
#include "ModuleSceneMain.h"


ElementGameUI::ElementGameUI(GameObject* parent, int x, int y, int h, int w, bool isVisible)
{
	gameObject = parent;
	visible = isVisible;
	rect.x = x;
	rect.y = y;
	rect.h = h;
	rect.w = w;
}

ElementGameUI::~ElementGameUI() {}

void ElementGameUI::drawGUI()
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
}

void ElementGameUI::startGUI()
{
}

void ElementGameUI::endGUI()
{
}
