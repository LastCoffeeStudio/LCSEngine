#include "Globals.h"
#include "Application.h"
#include "ModuleSceneMain.h"
#include "ModuleInput.h"
#include "CubeShape.h"
#include "SphereShape.h"
#include "ModuleTextures.h"
#include "Glew/include/glew.h"
#include "Imgui/imgui.h"
#include "SDL_mouse.h"

#define COUNT_LINES_GRID 100.f
#define POS_LINES_GRID COUNT_LINES_GRID / 2

ModuleSceneMain::ModuleSceneMain(bool active) : Module(active)
{
	cube1 = new CubeShape();
	sphere1 = new SphereShape(1.f, 16);
}

ModuleSceneMain::~ModuleSceneMain() {}

bool ModuleSceneMain::init()
{
	manualRotation = false;
	cube1->initializeValues();
	sphere1->initializeValues();
	App->textures->load("Assets/Images/iceSnow.jpg");
	return true;
}

bool ModuleSceneMain::start()
{
	actualPolygon = TRIANGLE;
	return true;
}

update_status ModuleSceneMain::update(const float deltaTime)
{
	//To check polygon rotation

	if (App->input->getKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		manualRotation = !manualRotation;
	}

	switch (manualRotation)
	{
	case true:
		if (App->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			int x = App->input->getMouseMotion().x;
			int y = App->input->getMouseMotion().y;
			if (MAX(abs(x), abs(y)) != 0)
			{
				float magnitude = sqrt((float)pow(x, 2) + (float)pow(y, 2));
				glRotatef(magnitude, (float)(x / MAX(abs(x), abs(y))), (float)(y / MAX(abs(x), abs(y))), 0.f);
			}
		}
		break;
	case false:
		//glRotatef(0.3f, 0.1f, 1.f, 0.f);
		break;
	}

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

	if (App->input->getKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		if (actualPolygon == CUBE)
		{
			cube1->changeRenderMode(true);
		}
		if (actualPolygon == SPHERE)
		{
			sphere1->changeRenderMode(true);
		}
	}
	if (App->input->getKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		if (actualPolygon == CUBE)
		{
			cube1->changeRenderMode(false);
		}
		if (actualPolygon == SPHERE)
		{
			sphere1->changeRenderMode(false);
		}
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

			//Back Face
			glColor3f(0.5f, 0.f, 0.f);
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

	for (unsigned int i = 0; i <= COUNT_LINES_GRID; i += DIST_BTW_LINES_GRID)
	{
		glBegin(GL_LINES);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-POS_LINES_GRID, 0.0f, (float)i - POS_LINES_GRID);
		glVertex3f(POS_LINES_GRID, 0.0f, (float)i - POS_LINES_GRID);
		glEnd();
		glBegin(GL_LINES);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f((float)i - POS_LINES_GRID, 0.0f, -POS_LINES_GRID);
		glVertex3f((float)i - POS_LINES_GRID, 0.0f, POS_LINES_GRID);
		glEnd();

	}
}