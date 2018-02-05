#include "MeshComponent.h"
#include "Imgui/imgui.h"

using namespace std;

static int selected_preset = -1;
const char* presets[] = { "Triangle", "Cube", "Sphere" };

MeshComponent::MeshComponent(GameObject* gameObject, bool isEnable, bool isUnique) : Component(gameObject, isEnable, isUnique)
{
	typeComponent = MESHCOMPONENT;
	verticesVBO.reserve(108);

	lengthX = lengthY = lengthZ = 1.f;

	verticesVBO = { lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f, -lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f, lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f,
		lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f, -lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f, -lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f,
		-lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f, -lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f, -lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f,
		-lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f, -lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f, -lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f,
		lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f, lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f, lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f,
		lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f, lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f, lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f,
		lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f, -lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f, lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f,
		lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f, -lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f, -lengthX / 2.f, lengthY / 2.f, lengthZ / 2.f,
		lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f, -lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f, lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f,
		lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f, -lengthX / 2.f, -lengthY / 2.f, lengthZ / 2.f, -lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f,
		-lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f, lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f, -lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f,
		-lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f, lengthX / 2.f, lengthY / 2.f, -lengthZ / 2.f, lengthX / 2.f, -lengthY / 2.f, -lengthZ / 2.f };


	glGenBuffers(1, (GLuint*) &(idVertVBO));
	glBindBuffer(GL_ARRAY_BUFFER, idVertVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesVBO.size(), &verticesVBO[0], GL_STATIC_DRAW);
}

MeshComponent::~MeshComponent() { }

bool MeshComponent::update()
{
	
	return true;
}


void MeshComponent::drawGUI()
{
	if (ImGui::CollapsingHeader("Mesh"))
	{
		ImGui::Checkbox("Active", &isEnable); ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(0.6f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(0.6f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(0.6f, 0.8f, 0.8f));
		if (ImGui::Button("Delete Component"))
		{
			suicide = true;
		}
		ImGui::TextColored({0.8f, 0.8f, 0.3f, 1.f}, selected_preset == -1 ? "<None>" : presets[selected_preset]);
		if (ImGui::Button("Change Preset.."))
		{
			ImGui::OpenPopup("select preset");
		}
		if (ImGui::BeginPopup("select preset"))
		{
			for (int i = 0; i < IM_ARRAYSIZE(presets); i++)
				if (ImGui::Selectable(presets[i]))
				{
					selected_preset = i;
					//Set Mesh of preset selected

				}
			ImGui::EndPopup();
		}
		ImGui::PopStyleColor(3);
		ImGui::TextColored({ 0.8f, 0.8f, 0.3f, 1.f }, "12 triangles (36 indices, 16 Vertices)");
	}
}
