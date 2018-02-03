#include "TransformComponent.h"
#include "Imgui/imgui.h"
#include <limits.h>

using namespace std;

TransformComponent::TransformComponent(GameObject* gameObject, string name, bool isEnable, bool isUnique) : Component(gameObject, name, isEnable, isUnique)
{
	typeComponent = TRANSFORM;
	transform.SetIdentity();
	matrixRotate.SetIdentity();
	matrixScale.SetIdentity();
	matrixTranslate.SetIdentity();
}

TransformComponent::~TransformComponent() { }

void TransformComponent::updateTransform()
{
	transform = matrixRotate*matrixScale*matrixTranslate;
}


void TransformComponent::drawGUI()
{
	if (ImGui::CollapsingHeader(name.c_str())) {
		if (ImGui::Checkbox(" ", &isEnable))
		{
			
		}
	}
	
	if (ImGui::SliderFloat3("Position", &position[0], -100.f/*numeric_limits<float>::min()*/, 100.f/*numeric_limits<float>::max()*/))
	{
		matrixTranslate.SetIdentity();
		matrixTranslate[0][3] = position[0];
		matrixTranslate[1][3] = position[1];
		matrixTranslate[2][3] = position[2];
		updateTransform();
		//matrixTranslate.SetIdentity();
	}

	if (ImGui::SliderFloat3("Sclale", &scale[0], 0.f/*numeric_limits<float>::min()*/, 100.f/*numeric_limits<float>::max()*/))
	{
		matrixScale.SetIdentity();
		matrixScale[0][0] = position[0];
		matrixScale[1][1] = position[1];
		matrixScale[2][2] = position[2];
		updateTransform();
	}
	
	if (ImGui::SliderAngle3("Rotation", &rotation[0]))
	{
		rotationQad = Quat::FromEulerXYZ(rotation.x, rotation.y, rotation.z);
		matrixRotate = rotationQad.ToFloat4x4();
	}
}
