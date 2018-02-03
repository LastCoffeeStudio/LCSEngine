#include "TransformComponent.h"
#include "Imgui/imgui.h"
#include <limits.h>

using namespace std;

TransformComponent::TransformComponent(GameObject* gameObject, bool isEnable, bool isUnique) : Component(gameObject, isEnable, isUnique)
{
	typeComponent = TRANSFORM;
}

TransformComponent::~TransformComponent() { }

void TransformComponent::drawGUI()
{
	if (ImGui::CollapsingHeader("Transform")) {
		if (ImGui::Checkbox(" ", &isEnable))
		{
			
		}
	}
	
	if (ImGui::SliderFloat3("Position", &position[0], -100.f/*numeric_limits<float>::min()*/, 100.f/*numeric_limits<float>::max()*/))
	{
		//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, alColor);
	}

	if (ImGui::SliderFloat3("Sclale", &scale[0], -100.f/*numeric_limits<float>::min()*/, 100.f/*numeric_limits<float>::max()*/))
	{
		//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, alColor);
	}
	
	if (ImGui::SliderAngle3("Rotation", &rotation[0]))
	{
		rotationQad = Quat::FromEulerXYZ(rotation.x, rotation.y, rotation.z);
	}

}
