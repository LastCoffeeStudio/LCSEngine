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
#include "MathGeoLib/src/Geometry/Circle.h"

ParticleSystemComponent::ParticleSystemComponent(GameObject* gameObject) : Component(gameObject, true, true)
{
	typeComponent = PARTICLESYSTEM;
	totalParticles = 1000;
	spawnTime = App->deltaTime;
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

	for (unsigned int i = 0; i < totalParticles; ++i)
	{
		Particle* partic = new Particle();
		partic->velocity = velocity;
		float3 position;
		getNewPosition(position);
		Billboard* billboard = new Billboard(position, 1.f, 1.f);
		partic->billboard = *billboard;
		partic->lifeTime = lifeTimeParticles;
		particles.push_back(*partic);
	}

	for (unsigned int i = 0; i < particles.size(); ++i)
	{
		inactiveParticles.push_back(&particles[i]);
	}

	/*TODO: set texture on GUI*/
	setTexture();
}

void ParticleSystemComponent::setTexture()
{
	map<std::string, AssetTexture*>::iterator itNewTexture = App->textures->textures.find(textureName);
	if (itNewTexture != App->textures->textures.end())
	{
		texID = (*itNewTexture).second->ID;
		(*itNewTexture).second->numberUsages++;
		hasTexture = true;
	}
	else if (App->textures->loadTexture(textureName))
	{
		texID = App->textures->textures[textureName]->ID;
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
		ImGui::PushID("texture");
		ImGui::Text("Texture:"); ImGui::SameLine(0);
		ImGui::PopID();
		ImGui::InputText("", &textureName[0], IM_ARRAYSIZE(textureName));
		if (ImGui::Button("Set texture"))
		{
			setTexture();
		}

		ImGui::Text("Time life particles: "); ImGui::SameLine();
		ImGui::PushID(3);
		ImGui::PushItemWidth(-1);
		ImGui::DragFloat("", &lifeTimeParticles,3.f);
		ImGui::PopItemWidth();
		ImGui::PopID();
		ImGui::PushID(4);
		ImGui::PushItemWidth(-1);
		ImGui::Text("Radio of component: "); ImGui::SameLine();
		ImGui::DragFloat("", &radious, 5.f);
		ImGui::PopItemWidth();
		ImGui::PopID();
		ImGui::PushID(5);
		ImGui::PushItemWidth(-1);
		ImGui::Text("Num particles: "); ImGui::SameLine();
		if(ImGui::DragInt("", &totalParticles, 1000))
		{
			particles.reserve(totalParticles);

			for (unsigned int i = 0; i < totalParticles; ++i)
			{
				Particle* partic = new Particle();
				partic->velocity = velocity;
				float3 position;
				getNewPosition(position);
				Billboard* billboard = new Billboard(position, 1.f, 1.f);
				partic->billboard = *billboard;
				partic->lifeTime = lifeTimeParticles;
				particles.push_back(*partic);
			}
			init();
		}
		ImGui::PopItemWidth();
		ImGui::PopID();
		ImGui::PushID(6);
		ImGui::PushItemWidth(-1);
		ImGui::Text("Velocity: "); ImGui::SameLine();
		if(ImGui::DragFloat3("", &velocity.x, 5.01f))
		{
			for (int i = 0; i < particles.size(); ++i)
			{
				particles[i].velocity = velocity;
			}
		}
		ImGui::PopItemWidth();
		ImGui::PopID();
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

	for (std::list<Particle*>::iterator it = activeParticles.begin(); it != activeParticles.end();)
	{
			vector<float3> vertexs;
			(*it)->billboard.ComputeQuad(&App->camera->currentCamera->frustum, vertexs);

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
			++it;
	}
}

void ParticleSystemComponent::updateParticles(float deltaTime)
{
	updateActiveParticles(deltaTime);
	spawnParticle(deltaTime);
	calculateVertexs();
}

void ParticleSystemComponent::getNewPosition(float3& newPosition)
{
	LCG lcg = LCG();
	float2::RandomDir(lcg);
	TransformComponent* transform = (TransformComponent*)(gameObject->getComponent(TRANSFORM));
	Circle circle = Circle(((TransformComponent*)(gameObject->getComponent(TRANSFORM)))->position, float3::unitY, radious);
	newPosition = circle.GetPoint(rand() % 360, (double)rand() / (double)RAND_MAX);
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
			float3 position;
			getNewPosition(position);
			(*it)->billboard.position = position;
			(*it)->lifeTime = lifeTimeParticles;
		}
		else
		{
			(*it)->billboard.position += (*it)->velocity * deltaTime;
			
		}
		++it;
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