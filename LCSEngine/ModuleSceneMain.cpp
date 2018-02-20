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
#include <list>
#include "MeshComponent.h"
#include "ComponentFactory.h"
#include "TransformComponent.h"

using namespace std;

#define COUNT_LINES_GRID 100.f
#define POS_LINES_GRID COUNT_LINES_GRID / 2

ModuleSceneMain::ModuleSceneMain(bool active) : Module(active)
{
	root = new GameObject();
	currentObject = root;
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
	float3 min = { 0.0f, 0.0f, 0.0f };
	float3 max = { 0.0f, 0.0f, 0.0f };
	AABB limits = AABB(min, max);
	quadtree->create(limits);
	return true;
}

bool ModuleSceneMain::start()
{
	return true;
}

update_status ModuleSceneMain::preUpdate(float deltaTime)
{
	BROFILER_CATEGORY("PreUpdateSceneMain", Profiler::Color::Orchid)
	clearGameObjects();
	clearComponents();
	preUpdateGameObjects();
	if (rebuildQuadTree)
	{
		makeQuadTree();
		rebuildQuadTree = false;
	}
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
	BROFILER_CATEGORY("ClearGameObjects", Profiler::Color::Orchid)
	
	bool erased = false;
	if (garbageCollector.size() > 0)
	{
		//delate parent pointer
		GameObject* gameObjectRoot = garbageCollector.front();
		for (vector<GameObject*>::iterator itDelete = gameObjectRoot->parent->children.begin(); itDelete != gameObjectRoot->parent->children.end() && !erased;)
		{
			if (*itDelete == gameObjectRoot)
			{
				itDelete = gameObjectRoot->parent->children.erase(itDelete);
				erased = true;
			}
			else
			{
				++itDelete;
			}
		}

		//Add all childrens to be deleted
		for (list<GameObject*>::iterator it = garbageCollector.begin(); it != garbageCollector.end(); ++it)
		{
			GameObject* gameObject = (GameObject*)*it;
			for (vector<GameObject*>::iterator it2 = gameObject->children.begin(); it2 != gameObject->children.end(); ++it2)
			{
				garbageCollector.push_back(*it2);
			}
		}

		for (list<GameObject*>::reverse_iterator it = garbageCollector.rbegin(); it != garbageCollector.rend(); ++it)
		{
			//Delete all components
			GameObject* gameObject = (GameObject*)*it;
			for (vector<Component*>::iterator itComponents = gameObject->components.begin(); itComponents != gameObject->components.end();)
			{
				RELEASE(*itComponents);
				itComponents = gameObject->components.erase(itComponents);
			}
			RELEASE(*it);
		}
		garbageCollector.clear();

		rebuildQuadTree = true;
	}
}

void ModuleSceneMain::clearComponents()
{
	bool ereased = false;
	for (list<Component*>::iterator it = garbageCollectorComponent.begin(); it != garbageCollectorComponent.end(); ++it)
	{
		Component* component = (Component*)*it;
		component->gameObject->deleteComponent(component);
		ereased = true;
	}
	if (ereased)
	{
		garbageCollectorComponent.clear();
	}
}

void ModuleSceneMain::preUpdateGameObjects()
{
	BROFILER_CATEGORY("PreUpdateGameObjects", Profiler::Color::Orchid)
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
	GLuint program = shader->programs[shader->defaultShaders[DEFAULTSHADER]];
	glUseProgram(program);

	float4x4 identity = float4x4::identity;

	GLint modelLoc = glGetUniformLocation(program, "model_matrix");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &identity[0][0]);

	GLint viewLoc = glGetUniformLocation(program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, App->camera->getViewMatrix());

	GLint projectLoc = glGetUniformLocation(program, "projection");
	glUniformMatrix4fv(projectLoc, 1, GL_FALSE, App->camera->getProjectMatrix());

	float3 cameraPos = App->camera->currentCamera->frustum.pos;

	glBegin(GL_LINES);

	//TODO: All vertices must be stored in VBO's, and calls to glVertex3f and glColor3f must be removed.
	if (Distance(float2(cameraPos.x, cameraPos.z), float2(0.0f, 0.0f)) < sqrt(pow(POS_LINES_GRID, 2) + pow(POS_LINES_GRID, 2)))
	{
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
	}
	
	for (unsigned int i = 0; i <= COUNT_LINES_GRID; i += DIST_BTW_LINES_GRID)
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		//Linies en X
		glVertex3f(-POS_LINES_GRID + floor(cameraPos.x), 0.0f, (float)i - POS_LINES_GRID + floor(cameraPos.z));
		glVertex3f(POS_LINES_GRID + floor(cameraPos.x), 0.0f, (float)i - POS_LINES_GRID + floor(cameraPos.z));
		glColor3f(1.0f, 1.0f, 1.0f);
		//Linies en Y
		glVertex3f((float)i - POS_LINES_GRID + floor(cameraPos.x), 0.0f, -POS_LINES_GRID + floor(cameraPos.z));
		glVertex3f((float)i - POS_LINES_GRID + floor(cameraPos.x), 0.0f, POS_LINES_GRID + floor(cameraPos.z));
	}
	glEnd();
}

void ModuleSceneMain::swapDefaultShader()
{
	//Keep this commented until we have a render queue, so it'll be easier to handle drawing ZBuffer on all the scene,
	//instead of changing program ID on every part of the code where glUseProgram is called.

	/*if (drawZbuffer)
	{
		drawZbuffer = false;
	}
	else
	{
		glUseProgram(shader->programs[shader->defaultShaders[ZBUFFERSHADER]]);
		drawZbuffer = true;
	}*/
}

void ModuleSceneMain::drawQuadTree()
{
	BROFILER_CATEGORY("DrawQuadTree", Profiler::Color::Orchid)
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
	GLuint program = shader->programs[shader->defaultShaders[DEFAULTSHADER]];

	glUseProgram(program);
	float4x4 identity = float4x4::identity;

	GLint modelLoc = glGetUniformLocation(program, "model_matrix");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &identity[0][0]);

	GLint viewLoc = glGetUniformLocation(program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, App->camera->getViewMatrix());

	GLint projectLoc = glGetUniformLocation(program, "projection");
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
}

void ModuleSceneMain::makeQuadTree()
{
	queue<GameObject*> queue;
	list<GameObject*> listStaticMeshObjects;
	for (vector<GameObject*>::iterator it = root->children.begin(); it != root->children.end(); ++it)
	{
		queue.push((*it));
	}

	while (!queue.empty())
	{
		GameObject* gameObject = queue.front();
		queue.pop();

		for (vector<Component*>::iterator it = gameObject->components.begin(); it != gameObject->components.end(); ++it)
		{
			if (MESH == (*it)->typeComponent &&gameObject->staticFlag == true)
			{
				listStaticMeshObjects.push_back(gameObject);
			}
		}

		for (vector<GameObject*>::iterator it = gameObject->children.begin(); it != gameObject->children.end(); ++it)
		{
			queue.push((*it));
		}
	}
	quadtree->insertAll(listStaticMeshObjects);
}