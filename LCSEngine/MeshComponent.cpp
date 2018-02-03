#include "MeshComponent.h"
#include "Imgui/imgui.h"

using namespace std;

MeshComponent::MeshComponent(GameObject* gameObject, string name, bool isEnable, bool isUnique) : Component(gameObject, name, isEnable, isUnique)
{
	typeComponent = MESHCOMPONENT;
}

MeshComponent::~MeshComponent() { }

void MeshComponent::drawGUI()
{
	if (ImGui::CollapsingHeader(name.c_str())) {
		if (ImGui::Checkbox(" ", &isEnable))
		{
			
		}
	}
}
