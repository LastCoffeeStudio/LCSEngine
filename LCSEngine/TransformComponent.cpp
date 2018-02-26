#include "TransformComponent.h"
#include "Imgui/imgui.h"
#include "GameObject.h"
#include "MathGeoLib/src/Math/MathFunc.h"
#include <limits.h>
#include "GameObject.h"

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

void TransformComponent::updateTranslate()
{
	matrixTranslate.SetIdentity();
	matrixTranslate[0][3] = position[0];
	matrixTranslate[1][3] = position[1];
	matrixTranslate[2][3] = position[2];
	updateTransform();
}

void TransformComponent::updateScale()
{
	matrixScale.SetIdentity();
	matrixScale[0][0] = scale[0];
	matrixScale[1][1] = scale[1];
	matrixScale[2][2] = scale[2];
	updateTransform();
}

void TransformComponent::updateRotate()
{
	float3 rotationRad = { DegToRad(rotation[0]), DegToRad(rotation[1]), DegToRad(rotation[2]) };
	rotationQad = Quat::FromEulerXYZ(rotationRad.x, rotationRad.y, rotationRad.z);
	matrixRotate = rotationQad.ToFloat4x4();
	updateTransform();
}

void TransformComponent::drawGUI()
{
	if (ImGui::CollapsingHeader("Transform"))
	{		
		if (gameObject->staticFlag) 
		{
			ImGui::Text("Position");
			ImGui::SameLine(78);
			ImGui::PushID("Position");
			ImGui::InputFloat3("", &position[0], -1, ImGuiInputTextFlags_ReadOnly);
			ImGui::PopID();

			ImGui::Text("Scale");
			ImGui::SameLine(70);
			ImGui::PushID("Scale");
			ImGui::InputFloat3("", &scale[0], -1, ImGuiInputTextFlags_ReadOnly);
			ImGui::PopID();

			ImGui::Text("Rotation");
			ImGui::SameLine(70);
			ImGui::PushID("Rotation");
			ImGui::InputFloat3("", &rotation[0], -1, ImGuiInputTextFlags_ReadOnly);
			ImGui::PopID();
		}
		else
		{
			ImGui::Text("Position");
			ImGui::SameLine(78);
			ImGui::PushID("Position");
			if (ImGui::DragFloat3("", &position[0], 0.1f))
			{
				updateTranslate();
			}
			ImGui::PopID();

			ImGui::Text("Scale");
			ImGui::SameLine(70);
			ImGui::PushID("Scale");
			if (ImGui::DragFloat3("", &scale[0], 0.1f))
			{
				updateScale();
			}
			ImGui::PopID();

			ImGui::Text("Rotation");
			ImGui::SameLine(70);
			ImGui::PushID("Rotation");
			if (ImGui::DragFloat3("", &rotation[0], 0.1f))
			{
				updateRotate();
			}
			ImGui::PopID();
		}
	}
}
