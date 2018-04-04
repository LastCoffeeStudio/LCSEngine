#include "LightComponent.h"
#include "GameObject.h"
#include "Application.h"
#include "Globals.h"
#include "Imgui/imgui.h"



LightComponent::LightComponent(GameObject* gameObject)
{
	typeComponent = LIGHT;
}

LightComponent::~LightComponent() {}

void LightComponent::drawGUI()
{
	if (ImGui::CollapsingHeader("Light"))
	{
	}
}


void LightComponent::load(nlohmann::json& conf)
{
	Component::load(conf);
	typeComponent = AUDIOSOURCE;
}

void LightComponent::save(nlohmann::json& conf)
{
	Component::save(conf);
	nlohmann::json customJsont;
}
