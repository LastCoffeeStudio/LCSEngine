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
	fontPath = "Assets/Fonts/MORPHEUS.ttf";
	App->type->loadFont(fontPath.c_str());

	static ElementFactory* factoryElements = ElementFactory::getInstance();
	textImage = (UIImage*)factoryElements->getComponent(IMAGE, nullptr, x, y, h, w, true);
}

UILabel::~UILabel() {}

void UILabel::drawGUI()
{
	if (ImGui::CollapsingHeader("Label"))
	{
		ElementGameUI::drawGUI();
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
		fontData = App->type->renderFont(text.c_str(), fontPath.c_str());

		glGenTextures(1, &fontData->idTexture);
		glBindTexture(GL_TEXTURE_2D, fontData->idTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fontData->width, fontData->height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, fontData->data);
		glBindTexture(GL_TEXTURE_2D, 0);
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
