#ifndef __PARTICLESYSTEMCOMPONENT_H__
#define __PARTICLESYSTEMCOMPONENT_H__

#include "Component.h"
#include "MathGeoLib/src/Math/float3.h"
#include <list>
#include "Billboard.h"

typedef unsigned int GLuint;

struct Particle
{
	Billboard billboard;
	float3 velocity = float3::zero;
	float lifeTime = 100.f;

	/*Particle()
	{
		billboard = Billboard{ float3::zero, 0.f, 0.f };
		velocity = float3::zero;
		lifeTime = 100.f;
	}*/
};

class ParticleSystemComponent : public Component
{
public:
	ParticleSystemComponent(GameObject* gameObject);
	~ParticleSystemComponent();
	void init();
	void drawGUI();
	void calculateVertexs();
	void updateBillboards();
	void updateParticles(float deltaTime);

public:
	std::vector<Particle> particles;
	std::list<Particle*> activeParticles;
	std::list<Particle*> inactiveParticles;
	unsigned int totalParticles = 0;
	float widthEmisor = 0.f;
	float heightEmisor = 0.f;
	float lifeTimeParticles = 0.f;
	float spawnTime = 0.f;
	float spawnCountdown = 0.f;
	GLuint sprite;

	GLuint idVertVBO = 0;
	GLuint idIdxVAO = 0;
	GLuint idTexCoords = 0;
	GLuint idColors = 0;
	GLuint texID = 0;
	bool hasTexture = false;
	std::vector<float3> verticesVBO;
	std::vector<float3> colorsVBO;
	std::vector<float2> texCoordsVBO;
	std::vector<unsigned int> indicesVBO;
	float minW = 0.9f;
	float maxW = 1.1f;
	float minH = 0.8f;
	float maxH = 1.2f;
	float quadSpaceX = 0.5f;
	float quadSpaceY = 0.5f;
private:
	void updateActiveParticles(float deltaTime);
	void spawnParticle(float deltaTime);

};

#endif //__PARTICLESYSTEMCOMPONENT_H__