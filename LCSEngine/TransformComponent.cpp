#include "TransformComponent.h"
#include "Imgui/imgui.h"
#include "GameObject.h"
#include "MathGeoLib/src/Math/MathFunc.h"
#include <limits.h>
#include "GameObject.h"
#include "SaveLoadManager.h"

TransformComponent::TransformComponent(GameObject* gameObject) : Component(gameObject, true, true)
{
	typeComponent = TRANSFORM;
	transform.SetIdentity();
	matrixRotate.SetIdentity();
	matrixScale.SetIdentity();
	matrixTranslate.SetIdentity();
}

TransformComponent::~TransformComponent() {}

void TransformComponent::setTransform(const aiMatrix4x4& matAiScene)
{
	for (unsigned int i = 0; i < 3; ++i)
	{
		position[i] = matAiScene[i][3];
		scale[i] = sqrt(pow(matAiScene[i][0], 2.f) + pow(matAiScene[i][1], 2.f) + pow(matAiScene[i][2], 2.f));
	}
	float3x3 rotMat = { matAiScene[0][0], matAiScene[0][1], matAiScene[0][2],
					    matAiScene[1][0], matAiScene[1][1], matAiScene[1][2],
					    matAiScene[2][0], matAiScene[2][1], matAiScene[2][2] };
	rotation = rotMat.ToEulerXYZ() * 180.f / pi;
	updateTranslate();
	updateScale();
	updateRotate();
}

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

	float3 up = rotationQad * float3::unitY;
}

void TransformComponent::setTranslate(const float3& positionVec)
{
	position = positionVec;
	updateTranslate();

}
void TransformComponent::setScale(const float3& scaleVec)
{
	scale = scaleVec;
	updateScale();
}

void TransformComponent::setRotate(const Quat& rotationQuat)
{
	float3 rotationRad = rotationQuat.ToEulerXYZ();
	rotation = rotationRad * 180.0f / pi;
	//rotation = rotationQuat.ToEulerXYZ();
	updateRotate();
}

float3 TransformComponent::up()
{
   return rotationQad * float3::unitY;
}

float3 TransformComponent::front()
{
    return rotationQad * float3::unitZ.Neg();
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

void TransformComponent::save(nlohmann::json& conf)
{
	
	Component::save(conf);
	nlohmann::json customJsont;
	SaveLoadManager::convertFloat3ToMyJSON(position, customJsont);
	conf["position"] = customJsont;
	SaveLoadManager::convertFloat3ToMyJSON(scale, customJsont);
	conf["scale"] = customJsont;
	SaveLoadManager::convertFloat3ToMyJSON(rotation, customJsont);
	conf["rotation"] = customJsont;
	SaveLoadManager::convertFloat4x4ToMyJSON(transform, customJsont);
	conf["transform"] = customJsont;
	SaveLoadManager::convertFloat4x4ToMyJSON(matrixTranslate, customJsont);
	conf["matrixTranslate"] = customJsont;
	SaveLoadManager::convertFloat4x4ToMyJSON(matrixScale, customJsont);
	conf["matrixScale"] = customJsont;
	SaveLoadManager::convertFloat4x4ToMyJSON(matrixRotate, customJsont);
	conf["matrixRotate"] = customJsont;
}

void TransformComponent::load(nlohmann::json& conf)
{
	Component::load(conf);
	SaveLoadManager::convertMyJSONtoFloat3(conf["position"], position);
	SaveLoadManager::convertMyJSONtoFloat3(conf["scale"], scale);
	SaveLoadManager::convertMyJSONtoFloat3(conf["rotation"], position);
	SaveLoadManager::convertMyJSONtoFloat4x4(conf["transform"], transform);
	SaveLoadManager::convertMyJSONtoFloat4x4(conf["matrixTranslate"], matrixTranslate);
	SaveLoadManager::convertMyJSONtoFloat4x4(conf["matrixScale"], matrixScale);
	SaveLoadManager::convertMyJSONtoFloat4x4(conf["matrixRotate"], matrixRotate);
	updateTransform();
}

