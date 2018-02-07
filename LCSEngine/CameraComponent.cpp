#include "CameraComponent.h"
#include "MathGeoLib/src/Math/MathFunc.h"
#include "MathGeoLib/src/Math/float3x4.h"
#include "ModuleCamera.h"
#include "Imgui/imgui.h"

CameraComponent::CameraComponent()
{
	frustum.type = PerspectiveFrustum;
	frustum.verticalFov = DegToRad(60.0f);
	frustum.horizontalFov = DegToRad(36.0f);
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.f;
	frustum.pos = float3::unitY;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
}

 
CameraComponent::~CameraComponent() { }

float* CameraComponent::getViewMatrix()
{
	return ((float4x4)frustum.ViewMatrix()).Transposed().v[0];
}

float* CameraComponent::getProjectMatrix()
{
	return frustum.ProjectionMatrix().Transposed().v[0];
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
			suicide = true;
		}
		ImGui::PopStyleColor(3);
		ImGui::SliderFloat("FOV Horizontal", &frustum.horizontalFov, 0.001f, 3.0f);
		ImGui::SliderFloat("FOV Vertical", &frustum.verticalFov, 0.001f, 3.0f);
		ImGui::SliderFloat("Near Plane Distance", &frustum.nearPlaneDistance, 0.001f, 100.0f);
		ImGui::SliderFloat("Far Plane Distance", &frustum.farPlaneDistance, 0.001f, 100.0f);
		//ImGui::SliderFloat("Movement Speed", &App->camera->cameraSpeed, 0.001f, 20.0f);
		//ImGui::SliderFloat("Rotation Speed", &App->camera->rotationSpeed, 0.001f, 20.0f);
		//ImGui::SliderFloat("Zoom Speed", &App->camera->zoomSpeed, 0.001f, 50.0f);

		//ImGui::SliderInt("Aspect Ratio", &App->window->height, 0.001f, 20.0f);
		ImGui::SliderFloat3("Front", (float*) &frustum.front, -10.0f, 10.0f);
		ImGui::SliderFloat3("Up", (float*) &frustum.up, -10.0f, 10.0f);
		ImGui::SliderFloat3("Position", (float*) &frustum.pos, -10.0f, 10.0f);
		//ImGui::ColorEdit3("Background Color", (float*)&clear_color);
		ImGui::Checkbox("Frustum culling", &frustumCulling);
		//ImGui::Checkbox("Wireframe Mode", &App->renderer->wireframe);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}

