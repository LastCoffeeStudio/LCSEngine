#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "BillboardGridComponent.h"
#include "Billboard.h"
#include "ModuleCamera.h"
#include "ModuleSceneMain.h"
#include "Imgui/imgui.h"
#include <glew.h>

BillboardGridComponent::BillboardGridComponent(GameObject* gameObject, bool isEnable, bool isUnique) : Component(gameObject, isEnable, isUnique)
{
	typeComponent = BILLBOARDGRID;

	glGenBuffers(1, (GLuint*) &(idVertVBO));
	glGenBuffers(1, (GLuint*) &(idIdxVAO));
	glGenBuffers(1, (GLuint*) &(idTexCoords));
	glGenBuffers(1, (GLuint*) &(idColors));
	billboards.reserve(n*m);

	float3 gameObjectPos = ((TransformComponent*)gameObject->getComponent(TRANSFORM))->position;

	for (unsigned int i = 0; i < n; ++i)
	{
		for (unsigned int j = 0; j < m; ++j)
		{
			float3 position = gameObjectPos + float3(1.f, 0.f, 0.f) * (quadSpaceX * j) + float3(0.f, 0.f, 1.f) * (quadSpaceY * i);
			Billboard* billboard = new Billboard(position, minW + float(rand() / float(RAND_MAX / (maxW-minW))), minH + float(rand() / float(RAND_MAX / (maxH - minH))));
			billboards.push_back(billboard);
		}
	}
}

BillboardGridComponent::~BillboardGridComponent() {}

void BillboardGridComponent::calculateVertexs()
{
	updateBillboards();

	glBindBuffer(GL_ARRAY_BUFFER, idVertVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesVBO.size() * 3, verticesVBO[0].ptr(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIdxVAO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesVBO.size(), &indicesVBO[0], GL_DYNAMIC_DRAW);

	if (texCoordsVBO.size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, idTexCoords);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texCoordsVBO.size() * 2, texCoordsVBO[0].ptr(), GL_DYNAMIC_DRAW);
	}

	colorsVBO.clear();
	for (unsigned int i = 0; i < verticesVBO.size(); ++i)
	{
		colorsVBO.push_back(float3(1.f, 0.f, 0.f));
	}

	glBindBuffer(GL_ARRAY_BUFFER, idColors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colorsVBO.size() * 3, colorsVBO[0].ptr(), GL_DYNAMIC_DRAW);
}

void BillboardGridComponent::drawGUI()
{
	if (ImGui::CollapsingHeader("Billboard Grid"))
	{
		ImGui::Checkbox("Active", &isEnable); ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(0.6f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(0.6f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(0.6f, 0.8f, 0.8f));
		if (ImGui::Button("Delete Component"))
		{
			App->sceneMain->garbageCollectorComponent.push_back(this);
		}
		ImGui::PopStyleColor(3);
	}
}

void BillboardGridComponent::updateBillboards()
{
	float3 gameObjectPos = ((TransformComponent*)gameObject->getComponent(TRANSFORM))->position;

	verticesVBO.clear();
	indicesVBO.clear();
	texCoordsVBO.clear();

	for (unsigned int i = 0; i < n; ++i)
	{
		for (unsigned int j = 0; j < m; ++j)
		{
			//billboards[i*m + j]->position = gameObjectPos + float3(1.f, 0.f, 0.f) * (quadSpaceX * j) + float3(0.f, 0.f, 1.f) * (quadSpaceY * i);
			vector<float3> vertexs;
			billboards[i*m + j]->ComputeQuad(&App->camera->currentCamera->frustum, vertexs);

			indicesVBO.push_back(1 + verticesVBO.size());
			indicesVBO.push_back(3 + verticesVBO.size());
			indicesVBO.push_back(2 + verticesVBO.size());
			indicesVBO.push_back(1 + verticesVBO.size());
			indicesVBO.push_back(0 + verticesVBO.size());
			indicesVBO.push_back(3 + verticesVBO.size());

			verticesVBO.push_back(vertexs[0]);
			verticesVBO.push_back(vertexs[1]);
			verticesVBO.push_back(vertexs[2]);
			verticesVBO.push_back(vertexs[3]);
		}
	}
}