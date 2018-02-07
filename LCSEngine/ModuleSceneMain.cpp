#include "Globals.h"
#include "Application.h"
#include "ModuleSceneMain.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "CubeShape.h"
#include "SphereShape.h"
#include "ModuleTextures.h"
#include "AssetTexture.h"
#include "Glew/include/glew.h"
#include "Imgui/imgui.h"
#include "SDL_mouse.h"
#include "Utils.h"
#include "DevIL/include/IL/il.h"
#include "Shader.h"
#include "GameObject.h"
#include "Component.h"
#include "MaterialComponent.h"
#include <queue>

/*TO DELETE*/
#include "TransformComponent.h"
#include "MeshComponent.h"

#define COUNT_LINES_GRID 100.f
#define POS_LINES_GRID COUNT_LINES_GRID / 2

ModuleSceneMain::ModuleSceneMain(bool active) : Module(active)
{
	/*
	cube1 = new CubeShape();
	sphere1 = new SphereShape(1.f, 16);
	*/

	root = new GameObject();
	root->addComponent(new TransformComponent(root));
	currentObject = root;
	/*This code is for testing purpose only, delete afterwards*/
	/*root->addGameObject(new GameObject(root, "omg"));
	root->addGameObject(new GameObject(root, "omg2"));
	root->addGameObject(new GameObject(root, "omg3"));
	root->addGameObject(new GameObject(root, "omg4"));
	root->children[0]->addGameObject(new GameObject(root->children[0], "wow"));
	root->children[0]->children[0]->addComponent(new MaterialComponent(root->children[0]->children[0]));*/
}

ModuleSceneMain::~ModuleSceneMain() {}

bool ModuleSceneMain::init()
{
	//cube1->initializeValues();
	//sphere1->initializeValues();
	checkers = App->textures->loadCheckers();
	lenna = App->textures->loadTexture(IL_PNG,"Assets/Images/Lenna.png");
	chocobo = App->textures->loadTexture(IL_JPG, "Assets/Images/chocobo.jpg");
	beer = App->textures->loadTexture(IL_DDS, "Assets/Images/beer.dds");
	actual = checkers;
	shader = new Shader();

	return true;
}

bool ModuleSceneMain::start()
{
	//actualPolygon = CUBE;
	/*root->addComponent(new TransformComponent(root));
	root->addComponent(new MeshComponent(root));*/
	return true;
}

update_status ModuleSceneMain::preUpdate(float deltaTime)
{
	omaeWaMouShindeiru();
	return UPDATE_CONTINUE;
}

update_status ModuleSceneMain::update(float deltaTime)
{
	return UPDATE_CONTINUE;
}

bool ModuleSceneMain::cleanUp()
{
	//cube1->cleanUp();
	//sphere1->cleanUp();

	return true;
}

void ModuleSceneMain::omaeWaMouShindeiru()
{
	queue<GameObject*> entities;

	for (vector<GameObject*>::iterator it = root->children.begin(); it != root->children.end(); ++it)
	{
		entities.push(*it);
	}

	while (!entities.empty())
	{
		GameObject* child = entities.front();
		entities.pop();
		
		for (vector<Component*>::iterator it = child->components.begin(); it != child->components.end();)
		{
			if ((*it)->suicide)
			{
				RELEASE(*it);
				it = child->components.erase(it);
			}
			else
			{
				++it;
			}
		}

		for (vector<GameObject*>::iterator it = child->children.begin(); it != child->children.end(); ++it)
		{
			entities.push(*it);
		}
	}
}

void ModuleSceneMain::draw()
{
	for (vector<GameObject*>::iterator it = root->children.begin(); it != root->children.end(); ++it)
	{
		if ((*it)->enable)
		{
			drawGameObjects((*it));
		}
	}
	drawGrid();
}

void ModuleSceneMain::drawGameObjects(GameObject* gameObject)
{
	gameObject->draw();
	for (vector<GameObject*>::iterator it = gameObject->children.begin(); it != gameObject->children.end(); ++it)
	{
		if ((*it)->enable)
		{
			drawGameObjects((*it));
		}
	}
}

void ModuleSceneMain::drawGrid()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	float4x4 id = float4x4::identity;

	GLint modelLoc = glGetUniformLocation(App->sceneMain->shader->shaderProgram, "model_matrix");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &id[0][0]);

	GLint viewLoc = glGetUniformLocation(App->sceneMain->shader->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, App->camera->getViewMatrix());

	GLint projectLoc = glGetUniformLocation(App->sceneMain->shader->shaderProgram, "projection");
	glUniformMatrix4fv(projectLoc, 1, GL_FALSE, App->camera->getProjectMatrix());
	//Draw grid
	float size = ImGui::GetWindowSize().x;
	if (ImGui::GetWindowSize().y > size) size = ImGui::GetWindowSize().y;

	float3 cameraPos = App->camera->frustum.pos;

	if (distance2(float2(cameraPos.x, cameraPos.z), float2(0.0f, 0.0f)) < sqrt(pow(POS_LINES_GRID, 2) + pow(POS_LINES_GRID, 2)))
	{
		glBegin(GL_LINES);
		// draw line for x axis
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(1.0, 0.0, 0.0);
		// draw line for y axis
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 1.0, 0.0);
		// draw line for Z axis
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 1.0);
		glEnd();
	}

	for (unsigned int i = 0; i <= COUNT_LINES_GRID; i += DIST_BTW_LINES_GRID)
	{
		glBegin(GL_LINES);
		glColor3f(1.0f, 1.0f, 1.0f);
		//Linies en X
		glVertex3f(-POS_LINES_GRID + floor(cameraPos.x), 0.0f, (float)i - POS_LINES_GRID + floor(cameraPos.z));
		glVertex3f(POS_LINES_GRID + floor(cameraPos.x), 0.0f, (float)i - POS_LINES_GRID + floor(cameraPos.z));
		glEnd();
		glBegin(GL_LINES);
		glColor3f(1.0f, 1.0f, 1.0f);
		//Linies en Y
		glVertex3f((float)i - POS_LINES_GRID + floor(cameraPos.x), 0.0f, -POS_LINES_GRID + floor(cameraPos.z));
		glVertex3f((float)i - POS_LINES_GRID + floor(cameraPos.x), 0.0f, POS_LINES_GRID + floor(cameraPos.z));
		glEnd();
	}

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindTexture(GL_TEXTURE_2D, 0);
}