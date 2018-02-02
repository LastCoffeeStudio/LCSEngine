#include "Component.h"
#include "GameObject.h"
#include "Imgui/imgui.h"

Component::Component() {}

Component::Component(GameObject* gameObject): gameObject(gameObject)
{
	typeComponent = COMPONENT;
	//TODO Set Checker in UI
}

Component::~Component()
{
	gameObject = nullptr;
}

bool Component::update()
{
	return true;
}

void Component::enable()
{
	//TODO Set Checker UI true and other commond thinks
}

void Component::disable()
{
	//TODO Set Checker UI false and other commond thinks
}

void Component::drawGUI() {}
