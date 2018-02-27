#include "Globals.h"
#include "Application.h"
#include "ModuleSceneMain.h"
#include "CameraComponent.h"
#include "MathGeoLib/src/Math/MathFunc.h"
#include "MathGeoLib/src/Math/float3x4.h"
#include "Imgui/imgui.h"

CameraComponent::CameraComponent(GameObject* gameObject, bool isEnable, bool isUnique) : Component(gameObject, isEnable, isUnique)
{
	typeComponent = CAMERA;
	frustum.type = PerspectiveFrustum;
	frustum.verticalFov = DegToRad(60.0f);
	frustum.horizontalFov = DegToRad(36.0f);
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 300.f;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
}

CameraComponent::~CameraComponent() {}

float* CameraComponent::getViewMatrix()
{
	return ((float4x4)frustum.ViewMatrix()).Transposed().v[0];
}

float* CameraComponent::getProjectMatrix()
{
	return frustum.ProjectionMatrix().Transposed().v[0];
}

float* CameraComponent::getViewMatrix4x4()
{
	return ((float4x4)frustum.ViewMatrix()).Transposed().ptr();
}

float4x4 CameraComponent::getViewProjectMatrix()
{
	return frustum.ViewProjMatrix();
}

void CameraComponent::drawGUI()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		ImGui::Checkbox("Active", &isEnable); ImGui::SameLine(0);
		ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(0.6f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(0.6f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(0.6f, 0.8f, 0.8f));
		if (ImGui::Button("Delete Component"))
		{
			App->sceneMain->garbageCollectorComponent.push_back(this);
		}
		ImGui::PopStyleColor(3);
		ImGui::SliderFloat("FOV Horizontal", &frustum.horizontalFov, 0.001f, 3.0f);
		ImGui::SliderFloat("FOV Vertical", &frustum.verticalFov, 0.001f, 3.0f);
		ImGui::SliderFloat("Near Plane Distance", &frustum.nearPlaneDistance, 0.001f, 100.0f);
		ImGui::SliderFloat("Far Plane Distance", &frustum.farPlaneDistance, 0.001f, 100.0f);
		ImGui::Checkbox("Frustum culling", &frustumCulling);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}