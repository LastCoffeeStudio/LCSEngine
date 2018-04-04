#include "ParticleSystemComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "AssetTexture.h"
#include "Billboard.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "AssetTexture.h"
#include "ModuleCamera.h"
#include "CameraComponent.h"
#include "Imgui/imgui.h"
#include "ModuleSceneMain.h"

ParticleSystemComponent::ParticleSystemComponent(GameObject* gameObject) : Component(gameObject, true, true)
{
	typeComponent = PARTICLESYSTEM;
	totalParticles = 20;
	init();
}


ParticleSystemComponent::~ParticleSystemComponent() {}



void ParticleSystemComponent::init()
{
	glGenBuffers(1, (GLuint*) &(idVertVBO));
	glGenBuffers(1, (GLuint*) &(idIdxVAO));
	glGenBuffers(1, (GLuint*) &(idTexCoords));
	glGenBuffers(1, (GLuint*) &(idColors));
	particles.reserve(totalParticles);

	float3 gameObjectPos = ((TransformComponent*)gameObject->getComponent(TRANSFORM))->position;

	for (unsigned int i = 0; i < 5; ++i)
	{
		for (unsigned int j = 0; j < 4; ++j)
		{
			float difX = -(quadSpaceX / 2.f) + float(rand() / float(RAND_MAX / quadSpaceX));
			float difY = -(quadSpaceY / 2.f) + float(rand() / float(RAND_MAX / quadSpaceY));
			float3 gameObjectPos = ((TransformComponent*)gameObject->getComponent(TRANSFORM))->position;
			float3 position = gameObjectPos + (float3(1.f, 0.f, 0.f) * ((quadSpaceX * j) + difX)) + float3(0.f, 0.f, 1.f) * ((quadSpaceY * i) + difY);
			/*Particle* partic = new Particle();
			partic->velocity = { 0.f, -0.1f, 0.f };
			Billboard* billboard = new Billboard({ 0.f,0.f,0.f }, minW + float(rand() / float(RAND_MAX / (maxW - minW))), minH + float(rand() / float(RAND_MAX / (maxH - minH))));
			partic->billboard = *billboard;
			particles.push_back(*partic);*/
		}

	}

	/*TODO: set texture on GUI*/
	map<std::string, AssetTexture*>::iterator itNewTexture = App->textures->textures.find("Assets/Images/billboardgrass.png");
	if (itNewTexture != App->textures->textures.end())
	{
		texID = (*itNewTexture).second->ID;
		(*itNewTexture).second->numberUsages++;
		hasTexture = true;
	}
	else if (App->textures->loadTexture("Assets/Images/billboardgrass.png"))
	{
		texID = App->textures->textures["Assets/Images/billboardgrass.png"]->ID;
		hasTexture = true;
	}
	else
	{
		texID = 0;
		hasTexture = false;
	}
}

void ParticleSystemComponent::drawGUI()
{
	if (ImGui::CollapsingHeader("ParticleSystem"))
	{
		ImGui::Checkbox("Active", &isEnable); ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(0.6f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(0.6f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(0.6f, 0.8f, 0.8f));
		if (ImGui::Button("Delete Component"))
		{
			App->sceneMain->garbageCollectorComponent.push_back(this);
		}
		ImGui::PopStyleColor(3);
	}
}


void ParticleSystemComponent::calculateVertexs()
{
	updateBillboards();

	glBindBuffer(GL_ARRAY_BUFFER, idVertVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesVBO.size() * 3, verticesVBO[0].ptr(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIdxVAO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesVBO.size(), &indicesVBO[0], GL_DYNAMIC_DRAW);



	if (texCoordsVBO.size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, idTexCoords);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texCoordsVBO.size() * 2, texCoordsVBO[0].ptr(), GL_DYNAMIC_DRAW);
	}

	colorsVBO.clear();
	for (unsigned int i = 0; i < verticesVBO.size(); ++i)
	{
		colorsVBO.push_back(float3(1.f, 1.f, 1.f));
	}

	glBindBuffer(GL_ARRAY_BUFFER, idColors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colorsVBO.size() * 3, colorsVBO[0].ptr(), GL_DYNAMIC_DRAW);
}


void ParticleSystemComponent::updateBillboards()
{
	float3 gameObjectPos = ((TransformComponent*)gameObject->getComponent(TRANSFORM))->position;

	verticesVBO.clear();
	indicesVBO.clear();
	texCoordsVBO.clear();

	for (unsigned int i = 0; i < totalParticles; ++i)
	{
			vector<float3> vertexs;
			particles[i].billboard.ComputeQuad(&App->camera->currentCamera->frustum, vertexs);

			indicesVBO.push_back(1 + verticesVBO.size());
			indicesVBO.push_back(3 + verticesVBO.size());
			indicesVBO.push_back(2 + verticesVBO.size());
			indicesVBO.push_back(1 + verticesVBO.size());
			indicesVBO.push_back(0 + verticesVBO.size());
			indicesVBO.push_back(3 + verticesVBO.size());

			verticesVBO.push_back(vertexs[0]);
			verticesVBO.push_back(vertexs[1]);
			verticesVBO.push_back(vertexs[2]);
			verticesVBO.push_back(vertexs[3]);

			texCoordsVBO.push_back(float2(0.f, 1.f));
			texCoordsVBO.push_back(float2(1.f, 1.f));
			texCoordsVBO.push_back(float2(1.f, 0.f));
			texCoordsVBO.push_back(float2(0.f, 0.f));
		}
}

void ParticleSystemComponent::updateParticles(float deltaTime)
{
	//Update Active particles
	//if number of active particles less than total particles && timeSpawn
		//Instantiate new particle from inactive particles
	//updateActiveParticles(deltaTime);
	//spawnParticle(deltaTime);
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
		//	(*it)->billboard.position += (*it)->velocity * deltaTime;
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