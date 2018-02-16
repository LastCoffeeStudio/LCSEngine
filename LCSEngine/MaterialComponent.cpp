#include "Application.h"
#include "ModuleSceneMain.h"
#include "MaterialComponent.h"
#include "Shader.h"
#include "Imgui/imgui.h"

MaterialComponent::MaterialComponent(GameObject* gameObject, bool isEnable) : Component(gameObject, isEnable, true)
{
	typeComponent = MATERIAL;
	shaderName = App->sceneMain->shader->defaultShaders[DEFAULTSHADER];
	program = App->sceneMain->shader->programs[shaderName];
}

MaterialComponent::~MaterialComponent() {}

void MaterialComponent::drawGUI()
{
	if (ImGui::CollapsingHeader("Material"))
	{
		ImGui::Checkbox("Active", &isEnable); ImGui::SameLine(0);
		ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(0.6f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(0.6f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(0.6f, 0.8f, 0.8f));
		if (ImGui::Button("Delete Component"))
		{
			suicide = true;
		}
		ImGui::PopStyleColor(3);
		ImGui::Text("Shader:"); ImGui::SameLine(0);
		ImGui::PushID(1);
		Shader* shader = App->sceneMain->shader;
		if (ImGui::BeginMenu(shaderName.c_str()))
		{
			for (vector<shaderInfo*>::iterator it = shader->shaders.begin(); it != shader->shaders.end(); ++it)
			{
				if (ImGui::MenuItem(((*it)->name).c_str()))
				{
					shaderName = (*it)->name;
					program = shader->programs[shaderName];
				}
			}
			ImGui::EndMenu();
		}
		ImGui::PopID();
		ImGui::PushID(2);
		ImGui::Text("Texture:"); ImGui::SameLine(0);
		//This should get all the textures availables and show them
		if (ImGui::BeginMenu(textureName.c_str()))
		{
			if (ImGui::MenuItem("Texture 1"))
			{
				textureName = "Texture 1";
			}
			if (ImGui::MenuItem("Texture 2"))
			{
				textureName = "Texture 2";
			}
			if (ImGui::MenuItem("Texture 3"))
			{
				textureName = "Texture 3";
			}
			if (ImGui::MenuItem("Texture 4"))
			{
				textureName = "Texture 4";
			}
			ImGui::EndMenu();
		}
		ImGui::PopID();
		ImGui::Text("Tiling: "); ImGui::SameLine();
		ImGui::PushID(3);
		ImGui::PushItemWidth(-1);
		ImGui::DragFloat2("", texTiling, 0.01f);
		ImGui::PopItemWidth();
		ImGui::PopID();
		ImGui::Text("Offset:"); ImGui::SameLine();
		ImGui::PushID(4);
		ImGui::PushItemWidth(-1);
		ImGui::DragFloat2("", texOffset, 0.01f);
		ImGui::PopItemWidth();
		ImGui::PopID();
		if (textureName != "None")
		{
			//ImGui::Image((void*)CALL_TO_TEXTURE_ID, { 64, 64 });
		}
	}
}