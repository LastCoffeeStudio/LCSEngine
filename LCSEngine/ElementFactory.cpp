#include "Globals.h"
#include "ElementFactory.h"
#include "ElementGameUI.h"
#include "GameObject.h"
#include "UIButton.h"
#include "UILabel.h"
#include "UIEditText.h"
#include "UIImage.h"

ElementFactory::~ElementFactory() {}

ElementFactory* elementFactory = nullptr;

ElementFactory* ElementFactory::getInstance()
{
	if (elementFactory == nullptr)
	{
		elementFactory = new ElementFactory();
	}
	return elementFactory;
}

ElementGameUI* ElementFactory::getComponent(TypeElemeneGametUI typeElement, GameObject* parentObject, int x, int y, int h, int w, bool isVisible)
{
	switch (typeElement)
	{
		case TypeElemeneGametUI::BUTTON:
			return new UIButton(parentObject, x, y, h, w, isVisible);
			break;
		case TypeElemeneGametUI::LABEL:
			return new UILabel(parentObject, x, y, h, w, isVisible);
			break;
		case TypeElemeneGametUI::IMAGE:
			return new UIImage(parentObject, x, y, h, w, isVisible);
			break;
		case TypeElemeneGametUI::EDITTEXT:
			return new UIEditText(parentObject, x, y, h, w, isVisible);
			break;
		default:
			return nullptr;
			break;
	}
}

ElementFactory::ElementFactory() {}