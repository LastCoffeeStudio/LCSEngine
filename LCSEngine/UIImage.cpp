#include "Globals.h"
#include "Application.h"
#include "ModuleSceneMain.h"
#include "ModuleWindow.h"
#include "ElementGameUI.h"
#include "UIImage.h"
#include "MathGeoLib/src/Math/MathFunc.h"
#include "MathGeoLib/src/Math/float3x4.h"
#include "Imgui/imgui.h"
#include "GameObject.h"

UIImage::UIImage(GameObject* parent, int x, int y, int h, int w, bool isVisible) : ElementGameUI(parent, x, y, h, w, isVisible)
{
	type = IMAGE;
	init();
	generateIDs();
}

UIImage::~UIImage() {}

void UIImage::drawGUI()
{
	if (ImGui::CollapsingHeader("Image"))
	{
		ImGui::Checkbox("Visible", &visible);
		ImGui::Checkbox("Enable", &enable);

		if (ImGui::Button("Delete Component"))
		{
			App->sceneMain->garbageCollectorElements.push_back(this);
		}

		ImGui::Text("Position");
		ImGui::PushID("position");
		if (ImGui::DragInt("X", &rect.x, 1))
		{
			updateCoords();
		}
		if (ImGui::DragInt("Y", &rect.y, 1))
		{
			updateCoords();
		}
		ImGui::PopID();

		ImGui::Text("Size");
		ImGui::PushID("size");
		if (ImGui::DragInt("Height", &rect.h, 1))
		{
			updateCoords();
		}
		if (ImGui::DragInt("Width", &rect.w, 1))
		{
			updateCoords();
		}
		ImGui::PopID();

		fillGUI();
	}
}

void UIImage::fillGUI()
{
	ImGui::PushID("texture");
	ImGui::Text("Texture:"); ImGui::SameLine(0);
	ImGui::PopID();
	ImGui::InputText("texture", &textureName[0], IM_ARRAYSIZE(textureName));
	if (ImGui::Button("Set texture") && textureName != texName)
	{
		textureChanged = true;
	}
}

void UIImage::init()
{
	float screenWidth = App->window->width;
	float screenHeight = App->window->height;
	float x1 = (float)((rect.x / screenWidth) * 2) - 1;
	float x2 = (float)(x1 + (rect.w / screenWidth) * 2);
	float y1 = (float)(2 - ((rect.y / screenHeight) * 2) - 1);
	//float y2 = (float)(2 - (y1 + ((*it)->rect.h) / (screenHeight / 2)));
	float y2 = (float)(y1 - (rect.h / screenHeight) * 2);

	verticesVBO.push_back(float3(x1, y1, 0.f));
	verticesVBO.push_back(float3(x1, y2, 0.f));
	verticesVBO.push_back(float3(x2, y2, 0.f));
	verticesVBO.push_back(float3(x2, y1, 0.f));
	indicesVAO.push_back(3);
	indicesVAO.push_back(1);
	indicesVAO.push_back(2);
	indicesVAO.push_back(3);
	indicesVAO.push_back(0);
	indicesVAO.push_back(1);
	texCoordsVBO.push_back(float2(0.f, 1.f));
	texCoordsVBO.push_back(float2(0.f, 0.f));
	texCoordsVBO.push_back(float2(1.f, 0.f));
	texCoordsVBO.push_back(float2(1.f, 1.f));

	/*verticesVBO = { float3(-1 / 2.f, 1 / 2.f, 1 / 2.f), float3(1 / 2.f, 1 / 2.f, 1 / 2.f),
		float3(1 / 2.f, -1 / 2.f, 1 / 2.f), float3(-1 / 2.f, -1 / 2.f, 1 / 2.f),
		float3(-1 / 2.f, 1 / 2.f, -1 / 2.f), float3(1 / 2.f, 1 / 2.f, -1 / 2.f),
		float3(1 / 2.f, -1 / 2.f, -1 / 2.f), float3(-1 / 2.f, -1 / 2.f, -1 / 2.f) };

	indicesVAO = { 1, 3, 2, 1, 0, 3,
		0, 7, 3, 0, 4, 7,
		5, 2, 6, 5, 1, 2,
		5, 0, 1, 5, 4, 0,
		2, 7, 6, 2, 3, 7,
		4, 6, 7, 4, 5, 6 };*/
}

void UIImage::generateIDs()
{
	glGenBuffers(1, (GLuint*) &(idVertVBO));
	glBindBuffer(GL_ARRAY_BUFFER, idVertVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesVBO.size() * 3, verticesVBO[0].ptr(), GL_DYNAMIC_DRAW);

	glGenBuffers(1, (GLuint*) &(idIdxVAO));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIdxVAO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesVAO.size(), &indicesVAO[0], GL_STATIC_DRAW);

	if (texCoordsVBO.size() > 0)
	{
		glGenBuffers(1, (GLuint*) &(idTexCoords));
		glBindBuffer(GL_ARRAY_BUFFER, idTexCoords);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texCoordsVBO.size() * 2, texCoordsVBO[0].ptr(), GL_STATIC_DRAW);
	}
	colorsVBO.clear();
	for (unsigned int i = 0; i < verticesVBO.size(); ++i)
	{
		colorsVBO.push_back(float3(1.f, 1.f, 1.f));
	}

	glGenBuffers(1, (GLuint*) &(idColors));
	glBindBuffer(GL_ARRAY_BUFFER, idColors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colorsVBO.size() * 3, colorsVBO[0].ptr(), GL_DYNAMIC_DRAW);
}

void UIImage::updateCoords()
{
	float screenWidth = App->window->width;
	float screenHeight = App->window->height;
	float x1 = (float)((rect.x / screenWidth) * 2) - 1;
	float x2 = (float)(x1 + (rect.w / screenWidth) * 2);
	float y1 = (float)(2 - ((rect.y / screenHeight) * 2) - 1);
	//float y2 = (float)(2 - (y1 + ((*it)->rect.h) / (screenHeight / 2)));
	float y2 = (float)(y1 - (rect.h / screenHeight) * 2);

	verticesVBO.clear();

	verticesVBO.push_back(float3(x1, y1, 0.f));
	verticesVBO.push_back(float3(x1, y2, 0.f));
	verticesVBO.push_back(float3(x2, y2, 0.f));
	verticesVBO.push_back(float3(x2, y1, 0.f));

	glBindBuffer(GL_ARRAY_BUFFER, idVertVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesVBO.size() * 3, verticesVBO[0].ptr(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}