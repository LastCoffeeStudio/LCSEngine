#include "Globals.h"
#include "Application.h"
#include "ModuleSceneMain.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "ModuleTextures.h"
#include "Component.h"
#include "CameraComponent.h"
#include "GameObject.h"
#include "CubeShape.h"
#include "SphereShape.h"
#include "AssetTexture.h"
#include "SDL_mouse.h"
#include "Shader.h"
#include "Glew/include/glew.h"
#include "Imgui/imgui.h"
#include "DevIL/include/IL/il.h"
#include "Brofiler.h"
#include "MathGeoLib/src/Math/MathFunc.h"
#include "QuadTree.h"
#include "SDL/include/SDL_assert.h"
#include <queue>

#define COUNT_LINES_GRID 100.f
#define POS_LINES_GRID COUNT_LINES_GRID / 2

ModuleSceneMain::ModuleSceneMain(bool active) : Module(active)
{
	root = new GameObject();
	currentObject = root;

	/*Needed for testing times*/
	/*for (int i = 0; i < 1000; ++i)
	{
		root->addGameObject(new GameObject(root,"a"));
	}*/
}

ModuleSceneMain::~ModuleSceneMain() {}

bool ModuleSceneMain::init()
{
	checkers = App->textures->loadCheckers();
	lenna = App->textures->loadTexture(IL_PNG, "Assets/Images/Lenna.png");
	chocobo = App->textures->loadTexture(IL_JPG, "Assets/Images/chocobo.jpg");
	beer = App->textures->loadTexture(IL_DDS, "Assets/Images/beer.dds");
	actual = checkers;
	shader = new Shader();
	quadtree = new QuadTree();
	const float3 min = { -20.0f, -20.0f, -20.0f };
	const float3 max = { 20.0f, 20.0f, 20.0f };
	limits = AABB(min, max);
	quadtree->create(limits);
	return true;
}

bool ModuleSceneMain::start()
{
	return true;
}

update_status ModuleSceneMain::preUpdate(float deltaTime)
{
	clearGameObjects();
	preUpdateGameObjects();
	return UPDATE_CONTINUE;
}

update_status ModuleSceneMain::update(float deltaTime)
{
	BROFILER_CATEGORY("UpdateSceneMain", Profiler::Color::Orchid)
	checkVisibleItems();
	return UPDATE_CONTINUE;
}

update_status ModuleSceneMain::postUpdate(float deltaTime)
{

	//postUpdateGameObjects();
	return UPDATE_CONTINUE;
}

bool ModuleSceneMain::cleanUp()
{
	return true;
}

void ModuleSceneMain::clearGameObjects()
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

		if (child->suicide == true)
		{
			bool erased = false;
			for (vector<GameObject*>::iterator it = child->parent->children.begin(); it != child->parent->children.end() && !erased;)
			{
				if ((*it) == child)
				{
					it = child->parent->children.erase(it);
					erased = true;
				}
				else
				{
					++it;
				}
			}
			RELEASE(child);
		}
		else
		{
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
}

void ModuleSceneMain::preUpdateGameObjects()
{
	queue<GameObject*> queue;

	for (vector<GameObject*>::iterator it = root->children.begin(); it != root->children.end(); ++it)
	{
		queue.push((*it));
	}

	while (!queue.empty())
	{
		GameObject* gameObject = queue.front();
		queue.pop();

		gameObject->preUpdate();

		for (vector<GameObject*>::iterator it = gameObject->children.begin(); it != gameObject->children.end(); ++it)
		{
			queue.push((*it));
		}
	}
}

void ModuleSceneMain::postUpdateGameObjects()
{
	queue<GameObject*> queue;

	for (vector<GameObject*>::iterator it = root->children.begin(); it != root->children.end(); ++it)
	{
		queue.push((*it));
	}

	while (!queue.empty())
	{
		GameObject* gameObject = queue.front();
		queue.pop();

		gameObject->postUpdate();

		for (vector<GameObject*>::iterator it = gameObject->children.begin(); it != gameObject->children.end(); ++it)
		{
			queue.push((*it));
		}
	}
}

void ModuleSceneMain::checkVisibleItems()
{
	vector<GameObject*> elements;
	vector<CameraComponent*> frustums;
	queue<GameObject*> queue;

	//We take all active frustums in the scene
	//And all the elements for fast iteration afterwars
	for (vector<GameObject*>::iterator it = root->children.begin(); it != root->children.end(); ++it)
	{
		if ((*it)->enable)
		{
			queue.push((*it));
		}
	}

	while (!queue.empty())
	{
		GameObject* gameObject = queue.front();
		queue.pop();

		elements.push_back(gameObject);

		for (vector<Component*>::iterator it = gameObject->components.begin(); it != gameObject->components.end(); ++it)
		{
			if ((*it)->typeComponent == CAMERA && (*it)->isEnable)
			{
				frustums.push_back((CameraComponent*)(*it));
			}
		}

		for (vector<GameObject*>::iterator it = gameObject->children.begin(); it != gameObject->children.end(); ++it)
		{
			if ((*it)->enable)
			{
				queue.push((*it));
			}
		}
	}

	//for each gameobject we check if any frustum makes this object visible. If it does, we continue with the next gameobject
	//If not, we make the gameobject as not visible
	for (vector<GameObject*>::iterator element = elements.begin(); element != elements.end(); ++element)
	{
		bool visible = false;
		(*element)->visible = true;
		for (vector<CameraComponent*>::iterator frustum = frustums.begin(); frustum != frustums.end() && !visible; ++frustum)
		{
			if (!(*frustum)->frustumCulling)
			{
				visible = true;
			}
			else
			{
				visible = (*frustum)->frustum.Intersects((*element)->obb.MinimalEnclosingAABB());
			}
		}
		if (frustums.size() > 0)
		{
			(*element)->visible = visible;
		}
	}
}

void ModuleSceneMain::draw()
{
	BROFILER_CATEGORY("DrawSceneMain", Profiler::Color::Orchid)
	for (vector<GameObject*>::iterator it = root->children.begin(); it != root->children.end(); ++it)
	{
		if ((*it)->enable)
		{
			drawGameObjects((*it));
		}
	}
	drawGrid();
	drawQuadTree();
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

	float3 cameraPos = App->camera->currentCamera->frustum.pos;

	if (Distance(float2(cameraPos.x, cameraPos.z), float2(0.0f, 0.0f)) < sqrt(pow(POS_LINES_GRID, 2) + pow(POS_LINES_GRID, 2)))
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

void ModuleSceneMain::swapDefaultShader()
{
	if (drawZbuffer)
	{
		shader->fshaderID = shader->readShader("Assets/Shaders/fragshader.txt", GL_FRAGMENT_SHADER);
		drawZbuffer = false;
	}
	else
	{
		shader->fshaderID = shader->readShader("Assets/Shaders/fragshaderZbuffer.txt", GL_FRAGMENT_SHADER);
		drawZbuffer = true;
	}
	shader->linkShaders();
	glUseProgram(shader->shaderProgram);
}

void ModuleSceneMain::drawQuadTree()
{
	queue<QuadNode*> nodes;
	SDL_assert(quadtree->root != nullptr);
	nodes.push(quadtree->root);

	while (!nodes.empty())
	{
		QuadNode* node = nodes.front();
		nodes.pop();

		drawAABB(node->limit);

		for (vector<QuadNode*>::iterator it = node->children.begin(); it != node->children.end(); ++it)
		{
			if ((*it) != nullptr)
			{
				nodes.push(*it);
			}
		}
	}
}

void ModuleSceneMain::drawAABB(const AABB& aabb)
{
	float4x4 identity = float4x4::identity;
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	GLint modelLoc = glGetUniformLocation(App->sceneMain->shader->shaderProgram, "model_matrix");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &identity[0][0]);

	GLint viewLoc = glGetUniformLocation(App->sceneMain->shader->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, App->camera->getViewMatrix());

	GLint projectLoc = glGetUniformLocation(App->sceneMain->shader->shaderProgram, "projection");
	glUniformMatrix4fv(projectLoc, 1, GL_FALSE, App->camera->getProjectMatrix());

	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(aabb.MinX(), aabb.MinY(), aabb.MinZ());
	glVertex3f(aabb.MaxX(), aabb.MinY(), aabb.MinZ());
	glVertex3f(aabb.MinX(), aabb.MinY(), aabb.MinZ());
	glVertex3f(aabb.MinX(), aabb.MaxY(), aabb.MinZ());
	glVertex3f(aabb.MinX(), aabb.MinY(), aabb.MinZ());
	glVertex3f(aabb.MinX(), aabb.MinY(), aabb.MaxZ());
	glVertex3f(aabb.MaxX(), aabb.MinY(), aabb.MinZ());
	glVertex3f(aabb.MaxX(), aabb.MaxY(), aabb.MinZ());
	glVertex3f(aabb.MaxX(), aabb.MinY(), aabb.MinZ());
	glVertex3f(aabb.MaxX(), aabb.MinY(), aabb.MaxZ());
	glVertex3f(aabb.MinX(), aabb.MaxY(), aabb.MinZ());
	glVertex3f(aabb.MaxX(), aabb.MaxY(), aabb.MinZ());
	glVertex3f(aabb.MinX(), aabb.MaxY(), aabb.MinZ());
	glVertex3f(aabb.MinX(), aabb.MaxY(), aabb.MaxZ());
	glVertex3f(aabb.MinX(), aabb.MinY(), aabb.MaxZ());
	glVertex3f(aabb.MaxX(), aabb.MinY(), aabb.MaxZ());
	glVertex3f(aabb.MinX(), aabb.MinY(), aabb.MaxZ());
	glVertex3f(aabb.MinX(), aabb.MaxY(), aabb.MaxZ());
	glVertex3f(aabb.MaxX(), aabb.MaxY(), aabb.MinZ());
	glVertex3f(aabb.MaxX(), aabb.MaxY(), aabb.MaxZ());
	glVertex3f(aabb.MaxX(), aabb.MinY(), aabb.MaxZ());
	glVertex3f(aabb.MaxX(), aabb.MaxY(), aabb.MaxZ());
	glVertex3f(aabb.MinX(), aabb.MaxY(), aabb.MaxZ());
	glVertex3f(aabb.MaxX(), aabb.MaxY(), aabb.MaxZ());
	glEnd();

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void ModuleSceneMain::makeQuadTree()
{
	queue<GameObject*> queue;
	quadtree->clear();
	quadtree = new QuadTree();
	quadtree->create(limits);

	for (vector<GameObject*>::iterator it = root->children.begin(); it != root->children.end(); ++it)
	{
		queue.push((*it));
	}

	while (!queue.empty())
	{
		GameObject* gameObject = queue.front();
		queue.pop();

		if(gameObject->staticFlag == true)
		{
			quadtree->insert(gameObject);
		}

		for (vector<GameObject*>::iterator it = gameObject->children.begin(); it != gameObject->children.end(); ++it)
		{
			queue.push((*it));
		}
	}
}