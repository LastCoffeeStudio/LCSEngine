#include "Application.h"
#include "ModuleSceneMain.h"
#include "ModuleScripts.h"
#include "ScriptComponent.h"
#include "Imgui/imgui.h"
#include <string>

ScriptComponent::ScriptComponent(GameObject* gameObject) : Component(gameObject, true)
{
	typeComponent = SCRIPT;
	App->scripts->addGameObject(gameObject);
}

ScriptComponent::~ScriptComponent() {}

void ScriptComponent::drawGUI()
{
	if (ImGui::CollapsingHeader("Script"))
	{
		ImGui::Checkbox("Active", &isEnable); ImGui::SameLine(0);
		ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(0.6f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(0.6f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(0.6f, 0.8f, 0.8f));
		if (ImGui::Button("Delete Component"))
		{
			App->sceneMain->garbageCollectorComponent.push_back(this);
		}
		ImGui::PopStyleColor(3);

		char aux2[128];
		strcpy_s(aux2, 128, scriptPath.c_str());
		ImGui::PushID("FontPath");
		if (ImGui::BeginMenu("script")) {

			if (ImGui::MenuItem("test.lua"))
			{
				scriptPath = "Assets/Scripts/test.lua";
			}
			ImGui::EndMenu();
		}
		ImGui::PopID();
	}
}

void ScriptComponent::updateScript()
{
	App->scripts->updateScript(scriptPath);
}
