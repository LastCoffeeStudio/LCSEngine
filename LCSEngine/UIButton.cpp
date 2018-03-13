#include "Globals.h"
#include "Application.h"
#include "ModuleSceneMain.h"
#include "ElementGameUI.h"
#include "UIButton.h"
#include "MathGeoLib/src/Math/MathFunc.h"
#include "MathGeoLib/src/Math/float3x4.h"
#include "Imgui/imgui.h"
#include "GameObject.h"
#include "UIImage.h"
#include "UILabel.h"
#include "ElementFactory.h"

UIButton::UIButton(GameObject* parent, int x, int y, int h, int w, bool isVisible) : ElementGameUI(parent, x, y, h, w, isVisible)
{
	type = BUTTON;
	static ElementFactory* factoryElements = ElementFactory::getInstance();
	label = (UILabel*)(factoryElements->getComponent(LABEL, nullptr, x, y, h, w, true));
	background = (UIImage*)(factoryElements->getComponent(IMAGE, nullptr, x, y, h, w, true));
}

UIButton::~UIButton() {}

void UIButton::drawGUI()
{
	
	if (ImGui::CollapsingHeader("Button"))
	{
		ElementGameUI::drawGUI();
		label->fillGUI();
	}
}

void UIButton::deleteElementButton()
{
	RELEASE(label);
	RELEASE(background);
}