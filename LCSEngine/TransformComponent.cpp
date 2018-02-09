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
		if (ImGui::SliderFloat3("Position", &position[0], -10.f/*numeric_limits<float>::min()*/, 10.f/*numeric_limits<float>::max()*/))
		{
			matrixTranslate.SetIdentity();
			matrixTranslate[0][3] = position[0];
			matrixTranslate[1][3] = position[1];
			matrixTranslate[2][3] = position[2];
			updateTransform();
		}

		if (ImGui::SliderFloat3("Scale", &scale[0], 0.f/*numeric_limits<float>::min()*/, 10.f/*numeric_limits<float>::max()*/))
		{
			matrixScale.SetIdentity();
			matrixScale[0][0] = scale[0];
			matrixScale[1][1] = scale[1];
			matrixScale[2][2] = scale[2];
			updateTransform();
		}

		if (ImGui::SliderAngle3("Rotation", &rotation[0]))
		{
			rotationQad = Quat::FromEulerXYZ(rotation.x, rotation.y, rotation.z);
			matrixRotate = rotationQad.ToFloat4x4();
			updateTransform();
		}
	}
}
