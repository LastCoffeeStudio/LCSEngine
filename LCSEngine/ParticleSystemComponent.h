#ifndef __PARTICLESYSTEMCOMPONENT_H__
#define __PARTICLESYSTEMCOMPONENT_H__

#include "Component.h"
#include "MathGeoLib/src/Math/float3.h"

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

public:
	std::vector<Particle*> particles;
	unsigned int totalParticles;
};

#endif //__PARTICLESYSTEMCOMPONENT_H__