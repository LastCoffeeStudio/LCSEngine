#include "Component.h"
#include "GameObject.h"
#include <list>

Component::Component() {}

Component::Component(GameObject* gameObject, bool isEnable, bool isUnique) : gameObject(gameObject), isEnable(isEnable), isUnique(isUnique)
{
    if(gameObject != nullptr)
    {
        UUIDparent = gameObject->UUID;
    }
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

void Component::load(nlohmann::json& conf)
{
	
}

void Component::save(nlohmann::json& conf)
{
	conf["isEnable"] = isEnable;
	conf["isUnique"] = isUnique;
	conf["typeComponent"] = typeComponent;
}

