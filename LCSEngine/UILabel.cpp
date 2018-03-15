#include "Globals.h"
#include "Application.h"
#include "ModuleSceneMain.h"
#include "ElementGameUI.h"
#include "UILabel.h"
#include "UIImage.h"
#include "MathGeoLib/src/Math/MathFunc.h"
#include "MathGeoLib/src/Math/float3x4.h"
#include "Imgui/imgui.h"
#include "GameObject.h"
#include "ModuleType.h"
#include "ElementFactory.h"

UILabel::UILabel(GameObject* parent, int x, int y, int h, int w, bool isVisible) : ElementGameUI(parent, x, y, h, w, isVisible)
{
	type = LABEL;
	text = "New Label";
	fontPath = "Assets/Fonts/Roboto-Regular.ttf";
	App->type->loadFont(fontPath.c_str());

	static ElementFactory* factoryElements = ElementFactory::getInstance();
	textImage = (UIImage*)factoryElements->getComponent(IMAGE, nullptr, x, y, h, w, true);

	glGenTextures(1, &idTexture);
}

UILabel::~UILabel() {}

void UILabel::drawGUI()
{
	if (ImGui::CollapsingHeader("Label"))
	{
		ImGui::Checkbox("Visible", &visible);
		ImGui::Checkbox("Enable", &enable);

		if (ImGui::Button("Delete Component"))
		{
			App->sceneMain->garbageCollectorElements.push_back(this);
		}

		ImGui::Text("Position");
		ImGui::PushID("position");
		ImGui::DragInt("X", &rect.x, 1);
		ImGui::DragInt("Y", &rect.y, 1);
		ImGui::PopID();

		ImGui::Text("Size");
		ImGui::PushID("size");
		ImGui::DragInt("Height", &rect.h, 1);
		ImGui::DragInt("Width", &rect.w, 1);
		ImGui::PopID();

		if (ImGui::ColorEdit4("color 2", textColor))
		{
			color.r = textColor[0] * 255;
			color.g = textColor[1] * 255;
			color.b = textColor[2] * 255;
			color.a = textColor[3] * 255;
			fillBufferData();
		}

		fillGUI();
	}
}

void UILabel::fillGUI()
{
	char aux1[64];
	strcpy_s(aux1, 64, text.c_str());

	ImGui::PushID("Text");
	if (ImGui::InputText("", aux1, 64))
	{
		text = aux1;
		fillBufferData();
	}
	ImGui::PopID();

	char aux2[128];
	strcpy_s(aux2, 128, fontPath.c_str());
	ImGui::PushID("FontPath");
	if (ImGui::InputText("", aux2, 128))
	{
		fontPath = aux2;
	}
	ImGui::PopID();

}

void UILabel::fillBufferData()
{
	fontData = App->type->renderFont(text.c_str(), fontPath.c_str(), color);

	fontData->idTexture = idTexture;
	glBindTexture(GL_TEXTURE_2D, fontData->idTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fontData->width, fontData->height, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, fontData->data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void UILabel::update()
{
	textImage->rect = rect;
	textImage->updateCoords();
}
