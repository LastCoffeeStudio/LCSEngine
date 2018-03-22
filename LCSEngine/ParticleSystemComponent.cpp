#include "ParticleSystemComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"

ParticleSystemComponent::ParticleSystemComponent(GameObject* gameObject) : Component(gameObject, true, true)
{
	typeComponent = PARTICLESYSTEM;
}


ParticleSystemComponent::~ParticleSystemComponent() {}

void ParticleSystemComponent::drawGUI() {}

void ParticleSystemComponent::updateParticles(float deltaTime)
{
	//Update Active particles
	//if number of active particles less than total particles && timeSpawn
		//Instantiate new particle from inactive particles
	updateActiveParticles(deltaTime);
	spawnParticle(deltaTime);
}

void ParticleSystemComponent::updateActiveParticles(float deltaTime)
{
	for (std::list<Particle*>::iterator it = activeParticles.begin(); it != activeParticles.end();)
	{
		(*it)->lifeTime -= deltaTime;
		if ((*it)->lifeTime < 0.f)
		{
			(*it)->lifeTime = lifeTimeParticles;
			float3 posGameObject = ((TransformComponent*)gameObject->getComponent(TRANSFORM))->position;
			//(*it)->position =
			inactiveParticles.push_back(*it);
			it = activeParticles.erase(it);
		}
		else
		{
			(*it)->position += (*it)->velocity * deltaTime;
			++it;
		}
	}
}

void ParticleSystemComponent::spawnParticle(float deltaTime)
{
	spawnCountdown -= deltaTime;
	if (spawnCountdown < 0.f && inactiveParticles.size() > 0 && activeParticles.size() < totalParticles)
	{
		activeParticles.push_back(inactiveParticles.front());
		inactiveParticles.pop_front();
		spawnCountdown = spawnTime;
	}
}