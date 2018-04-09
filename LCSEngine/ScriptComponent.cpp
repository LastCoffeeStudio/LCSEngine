#include "Application.h"
#include "ModuleSceneMain.h"
#include "ScriptComponent.h"
#include "Imgui/imgui.h"



#include <string>

ScriptComponent::ScriptComponent(GameObject* gameObject)
{
	typeComponent = SCRIPT;

	lua.open_libraries(sol::lib::base);
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
		if (ImGui::InputText("", aux2, 128))
		{
			scriptPath = aux2;
		}
		ImGui::PopID();
		if (ImGui::Button("Load"))
		{
			lua.script_file(scriptPath);
		}

	}
}

void ScriptComponent::updateScripts()
{
	sol::function fx = lua["update"];
	if (fx)
	{
		std::function<void()> stdfx = fx;
		stdfx();
	}
	
}

