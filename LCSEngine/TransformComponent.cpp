#include "TransformComponent.h"
#include "Imgui/imgui.h"
#include <limits.h>

TransformComponent::TransformComponent(GameObject* gameObject) : Component(gameObject, true, true)
{
	typeComponent = TRANSFORM;
	transform.SetIdentity();
	matrixRotate.SetIdentity();
	matrixScale.SetIdentity();
	matrixTranslate.SetIdentity();
}

TransformComponent::~TransformComponent() {}

void TransformComponent::updateTransform()
{
	transform = matrixTranslate*matrixRotate*matrixScale;
}

void TransformComponent::drawGUI()
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::Text("Position");
		ImGui::SameLine(78);
		ImGui::PushID("Position");
		if (ImGui::DragFloat3("", &position[0], 0.1f))
		{
			matrixTranslate.SetIdentity();
			matrixTranslate[0][3] = position[0];
			matrixTranslate[1][3] = position[1];
			matrixTranslate[2][3] = position[2];
			updateTransform();
		}
		ImGui::PopID();

		ImGui::Text("Scale");
		ImGui::SameLine(70);
		ImGui::PushID("Scale");
		if (ImGui::DragFloat3("", &scale[0], 0.1f))
		{
			matrixScale.SetIdentity();
			matrixScale[0][0] = scale[0];
			matrixScale[1][1] = scale[1];
			matrixScale[2][2] = scale[2];
			updateTransform();
		}
		ImGui::PopID();

		ImGui::Text("Rotation");
		ImGui::SameLine(70);
		ImGui::PushID("Rotation");
		if (ImGui::DragFloat3("", &rotation[0], 0.1f))
		{
			float3 rotationRad = { DegToRad(rotation[0]), DegToRad(rotation[1]), DegToRad(rotation[2]) };
			rotationQad = Quat::FromEulerXYZ(rotationRad.x, rotationRad.y, rotationRad.z);
			matrixRotate = rotationQad.ToFloat4x4();
			updateTransform();
		}
		ImGui::PopID();
	}
}
