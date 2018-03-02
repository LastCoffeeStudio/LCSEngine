#include "Globals.h"
#include "ComponentFactory.h"
#include "Component.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"
#include "CameraComponent.h"
#include "AnimationComponent.h"
#include "GameObject.h"

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
	case TypeComponent::ANIMATION:
		return new AnimationComponent(parentObject);
		break;
	default:
		return nullptr;
		break;
	}
}

ComponentFactory::ComponentFactory() {}