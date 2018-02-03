#include "MeshComponent.h"
#include "Imgui/imgui.h"

using namespace std;

MeshComponent::MeshComponent(GameObject* gameObject, bool isEnable, bool isUnique) : Component(gameObject, isEnable, isUnique)
{
	typeComponent = MESHCOMPONENT;
}

MeshComponent::~MeshComponent() { }

void MeshComponent::drawGUI()
{
	if (ImGui::CollapsingHeader("Mesh")) {
		if (ImGui::Checkbox(" ", &isEnable))
		{
			
		}
	}
}
