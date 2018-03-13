#include "Globals.h"
#include "Application.h"
#include "ModuleSceneMain.h"
#include "ElementGameUI.h"
#include "UIEditText.h"
#include "MathGeoLib/src/Math/MathFunc.h"
#include "MathGeoLib/src/Math/float3x4.h"
#include "Imgui/imgui.h"
#include "GameObject.h"

UIEditText::UIEditText(GameObject* parent, int x, int y, int h, int w, bool isVisible) : ElementGameUI(parent, x, y, h, w, isVisible)
{
	type = EDITTEXT;
}

UIEditText::~UIEditText() {}

void UIEditText::drawGUI()
{
	if (ImGui::CollapsingHeader("EditText"))
	{
		ElementGameUI::drawGUI();
	}
}