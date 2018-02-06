#include "Component.h"
#include "Imgui/imgui.h"

using namespace std;

Component::Component() {}

Component::Component(GameObject* gameObject, bool isEnable, bool isUnique) : gameObject(gameObject), isEnable(isEnable), isUnique(isUnique)
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

