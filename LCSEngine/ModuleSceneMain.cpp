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

#define COUNT_LINES_GRID 100.f
#define POS_LINES_GRID COUNT_LINES_GRID / 2

ModuleSceneMain::ModuleSceneMain(bool active) : Module(active)
{
	root = new GameObject();
	cube1 = new CubeShape();
	sphere1 = new SphereShape(1.f, 16);
}

ModuleSceneMain::~ModuleSceneMain() {}

bool ModuleSceneMain::init()
{
	cube1->initializeValues();
	sphere1->initializeValues();
	currentObject = root;
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
	actualPolygon = CUBE;
	return true;
}

update_status ModuleSceneMain::update(float deltaTime)
{
	if (App->input->getKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		actualPolygon = TRIANGLE;
	}
	if (App->input->getKey(SDL_SCANCODE_N) == KEY_DOWN)
	{
		actualPolygon = CUBE;
	}
	if (App->input->getKey(SDL_SCANCODE_B) == KEY_DOWN)
	{
		actualPolygon = SPHERE;
	}

	return UPDATE_CONTINUE;
}

bool ModuleSceneMain::cleanUp()
{
	cube1->cleanUp();
	sphere1->cleanUp();

	return true;
}

void ModuleSceneMain::draw()
{
	switch (actualPolygon)
	{
	case TRIANGLE:
		glBegin(GL_TRIANGLES);

		//Front Face
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(0.f, 0.5f, 0.f); // lower left vertex
		glVertex3f(-0.5f, -0.5f, 0.f); // lower right vertex
		glVertex3f(0.5f, -0.5f, 0.f); // upper vertex

		glColor3f(0.5f, 0.f, 0.f);	//Back Face
		glVertex3f(0.f, 0.5f, 0.f); // lower left vertex
		glVertex3f(0.5f, -0.5f, 0.f); // lower right vertex
		glVertex3f(-0.5f, -0.5f, 0.f); // upper vertex

		glEnd();
		break;

	case CUBE:
		cube1->draw();
		break;

	case SPHERE:
		sphere1->draw();
		break;
	}
	drawGrid();
}

void ModuleSceneMain::drawGrid()
{
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
}