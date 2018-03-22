#ifndef __PARTICLESYSTEMCOMPONENT_H__
#define __PARTICLESYSTEMCOMPONENT_H__

#include "Component.h"
#include "MathGeoLib/src/Math/float3.h"
#include <list>

typedef unsigned int GLuint;

struct Particle
{
	float3 position = float3::zero;
	float3 velocity = float3::zero;
	float lifeTime = 0.f;
};

class ParticleSystemComponent : public Component
{
public:
	ParticleSystemComponent(GameObject* gameObject);
	~ParticleSystemComponent();
	void drawGUI();
	void updateParticles(float deltaTime);

public:
	std::vector<Particle*> particles;
	std::list<Particle*> activeParticles;
	std::list<Particle*> inactiveParticles;
	unsigned int totalParticles = 0;
	float widthEmisor = 0.f;
	float heightEmisor = 0.f;
	float lifeTimeParticles = 0.f;
	float spawnTime = 0.f;
	float spawnCountdown = 0.f;
	GLuint sprite;

private:
	void updateActiveParticles(float deltaTime);
	void spawnParticle(float deltaTime);

};

#endif //__PARTICLESYSTEMCOMPONENT_H__