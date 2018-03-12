#include "Globals.h"
#include "Application.h"
#include "ModuleSceneMain.h"
#include "ElementGameUI.h"
#include "UILabel.h"
#include "MathGeoLib/src/Math/MathFunc.h"
#include "MathGeoLib/src/Math/float3x4.h"
#include "Imgui/imgui.h"

UILabel::UILabel(GameObject* parent, int x, int y, int h, int w, bool isVisible) : ElementGameUI(parent, x, y, h, w, isVisible)
{
	type = LABEL;
}

UILabel::~UILabel() {}

void UILabel::drawGUI()
{
	if (ImGui::CollapsingHeader("Label"))
	{
		ImGui::Checkbox("Visible", &visible);
	}
}