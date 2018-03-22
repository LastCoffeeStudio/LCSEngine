#ifndef __PARTICLESYSTEMCOMPONENT_H__
#define __PARTICLESYSTEMCOMPONENT_H__

#include "MathGeoLib/src/Math/float3.h"

typedef unsigned int GLuint;

struct Particle
{
	float3 position = float3::zero;
	float3 velocity = float3::zero;
	float width = 0.f;
	float height = 0.f;
	float lifeTime = 0.f;
	float3 color = float3(1.f,1.f,1.f);
	GLuint sprite;

};

class ParticleSystemComponent
{
public:
	ParticleSystemComponent();
	~ParticleSystemComponent();
};

#endif //__PARTICLESYSTEMCOMPONENT_H__