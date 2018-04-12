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
	float lifeTime = 10.f;

	Particle()
	{
		billboard = Billboard();
		velocity = float3::zero;
		lifeTime = 10.f;
	};
};

class ParticleSystemComponent : public Component
{
public:
	ParticleSystemComponent(GameObject* gameObject);
	~ParticleSystemComponent();
	void init();
	void setTexture();
	void drawGUI();
	void calculateVertexs();
	void updateBillboards();
	void updateParticles(float deltaTime);
	void getNewPosition(float3 &newPosition);

public:
	std::vector<Particle> particles;
	std::list<Particle*> activeParticles;
	std::list<Particle*> inactiveParticles;
	int totalParticles = 0;
	float widthEmisor = 0.f;
	float heightEmisor = 0.f;
	float lifeTimeParticles = 3.f;
	float spawnTime = 0.f;
	float spawnCountdown = 0.f;
	float3 velocity = { 0.f, -50.f, 0.f };
	GLuint sprite;
	float radious = 10.f;
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
	char textureName[128] = "Assets/Images/rainSprite.tga";
	
private:
	void updateActiveParticles(float deltaTime);
	void spawnParticle(float deltaTime);

private:

};

#endif //__PARTICLESYSTEMCOMPONENT_H__