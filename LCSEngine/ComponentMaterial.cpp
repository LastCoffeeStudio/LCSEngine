#include "ComponentMaterial.h"
#include "Imgui/imgui.h"

ComponentMaterial::ComponentMaterial(GameObject* gameObject, bool isEnable) : Component(gameObject,"", isEnable)
{
	typeComponent = MATERIAL;
}

ComponentMaterial::~ComponentMaterial() {}

void ComponentMaterial::drawGUI()
{
	if (ImGui::CollapsingHeader("Material"))
	{
		ImGui::Checkbox("Active", &isEnable); ImGui::SameLine(0);
		ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(0.6f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(0.6f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(0.6f, 0.8f, 0.8f));
		ImGui::Button("Delete Component");
		ImGui::PopStyleColor(3);
		ImGui::Text("Shader:"); ImGui::SameLine(0);
		if (ImGui::BeginMenu(shaderName.c_str()))
		{
			if (ImGui::MenuItem("Shader 1"))
			{
				shaderName = "Shader 1";
			}
			if (ImGui::MenuItem("Shader 2"))
			{
				shaderName = "Shader 2";
			}
			if (ImGui::MenuItem("Shader 3"))
			{
				shaderName = "Shader 3";
			}
			if (ImGui::MenuItem("Shader 4"))
			{
				shaderName = "Shader 4";
			}
			ImGui::EndMenu();
		}
	}
}