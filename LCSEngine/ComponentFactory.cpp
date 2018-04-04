#include "Globals.h"
#include "ComponentFactory.h"
#include "Component.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"
#include "CameraComponent.h"
#include "AnimationComponent.h"
#include "GameObject.h"
#include "AudioListenerComponent.h"
#include "AudioSourceComponent.h"
#include "BillboardGridComponent.h"
#include "ParticleSystemComponent.h"
#include "LightComponent.h"

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
    case TypeComponent::AUDIOLISTENER:
        return new AudioListenerComponent(parentObject);
        break;
    case TypeComponent::AUDIOSOURCE:
        return new AudioSourceComponent(parentObject);
        break;
	case TypeComponent::PARTICLESYSTEM:
		return new ParticleSystemComponent(parentObject);
		break;
	case TypeComponent::BILLBOARDGRID:
		return new BillboardGridComponent(parentObject);
		break;
	case TypeComponent::LIGHT:
		return new LightComponent(parentObject);
		break;
	default:
		return nullptr;
		break;
	}
}

ComponentFactory::ComponentFactory() {}