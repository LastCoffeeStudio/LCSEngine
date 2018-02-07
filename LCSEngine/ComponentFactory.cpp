#include "ComponentFactory.h"
#include "Component.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"
#include "CameraComponent.h"
#include "GameObject.h"
#include "Globals.h"

ComponentFactory::~ComponentFactory() {}


ComponentFactory* componentFactory = nullptr;

ComponentFactory* ComponentFactory::getInstance()
{
	if (componentFactory == nullptr) 
	{
		componentFactory = new ComponentFactory();
	}
	return componentFactory;
}

Component* ComponentFactory::getComponent(TypeComponent typeComponent, GameObject* parentObject) 
{
	switch (typeComponent)
	{
	case TypeComponent::MESH:
		return new MeshComponent(parentObject);
		break;
	case TypeComponent::TRANSFORM:
		return new TransformComponent(parentObject);
		break;
	case TypeComponent::MATERIAL:
		return new MaterialComponent(parentObject);
		break;
	case TypeComponent::CAMERA:
		return new CameraComponent(parentObject);
		break;
	default:
		break;
	}
	
}

ComponentFactory::ComponentFactory() {}