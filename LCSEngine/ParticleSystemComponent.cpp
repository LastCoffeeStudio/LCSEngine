#include "ParticleSystemComponent.h"

ParticleSystemComponent::ParticleSystemComponent(GameObject* gameObject) : Component(gameObject, true, true)
{
	typeComponent = PARTICLESYSTEM;
}


ParticleSystemComponent::~ParticleSystemComponent()
{
}
