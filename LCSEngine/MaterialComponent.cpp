#include "Application.h"
#include "ModuleSceneMain.h"
#include "ModuleTextures.h"
#include "MaterialComponent.h"
#include "Shader.h"
#include "Imgui/imgui.h"
#include <filesystem>
#include "SaveLoadManager.h"

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
			App->sceneMain->garbageCollectorComponent.push_back(this);
		}
		ImGui::PopStyleColor(3);
		ImGui::Text("Shader:"); ImGui::SameLine(0);
		ImGui::PushID("shader");
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
		ImGui::PushID("texture");
		ImGui::Text("Texture:"); ImGui::SameLine(0);
		//This should get all the textures availables and show them
		/*if (ImGui::BeginMenu(textureName.c_str()))
		{
			if (ImGui::MenuItem("None"))
			{
				textureName = "None";
			}
			for (map<string, AssetTexture*>::iterator it = App->textures->textures.begin(); it != App->textures->textures.end(); ++it)
			{
				if (ImGui::MenuItem((*it).first.c_str()))
				{
					textureName = (*it).first.c_str();
				}
			}
			

			ImGui::EndMenu();
		}*/
		ImGui::PopID();

		ImGui::InputText("", &textureName[0], IM_ARRAYSIZE(textureName));
		if (ImGui::Button("Set texture"))
		{
			textureChanged = true;
		}
		
		ImGui::PushID("color");
		if (ImGui::ColorEdit3("Color", &color[0]))
		{
			colorChanged = true;
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

void MaterialComponent::setNameTexture(string name)
{
	strcpy_s(textureName, name.c_str());
}

void MaterialComponent::load(nlohmann::json& conf)
{
	Component::load(conf);
	typeComponent = MATERIAL;
	SaveLoadManager::convertMyJSONtoFloat3(conf["color"], color);
	string texName = conf.at("textureName").get<std::string>().c_str();
	strcpy_s(textureName, 128, texName.c_str());
	textureChanged = true;
	program = App->sceneMain->shader->programs[shaderName];
}

void MaterialComponent::save(nlohmann::json& conf)
{
	Component::save(conf);
	nlohmann::json customJsont;
	conf["textureName"] = textureName;
	conf["shaderName"] = shaderName;
	SaveLoadManager::convertFloat3ToMyJSON(color, customJsont);
	conf["color"] = customJsont;
}