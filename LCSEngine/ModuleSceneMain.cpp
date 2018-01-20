#include "Globals.h"
#include "Application.h"
#include "ModuleSceneMain.h"
#include "ModuleInput.h"
#include "CubeShape.h"
#include "SphereShape.h"
#include "Glew/include/glew.h"
#include "SDL_mouse.h"

#include <iostream>
using namespace std;

ModuleSceneMain::ModuleSceneMain(bool active) : Module(active)
{
	cube1 = new CubeShape();
	sphere1 = new SphereShape();
}

ModuleSceneMain::~ModuleSceneMain() {}

bool ModuleSceneMain::init()
{
	cube1->initializeValues();
	sphere1->initializeValues();
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
	glRotatef(0.3f, 0.1f, 1.f, 0.f);

	/*if (App->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		int x = App->input->getMouseMotion().x;
		int y = App->input->getMouseMotion().y;
		if (MAX(abs(x), abs(y)) != 0)
		{
			float magnitude = sqrt((float)pow(x, 2) + (float)pow(y, 2));
			glRotatef(magnitude, (float)(x / MAX(abs(x), abs(y))), (float)(y / MAX(abs(x), abs(y))), 0.f);
			cout << magnitude << " " << x << " " << y << endl;
		}
	}*/

	if (App->input->getKey(SDL_SCANCODE_T) == KEY_DOWN)
	{
		actualPolygon = TRIANGLE;
	}
	if (App->input->getKey(SDL_SCANCODE_C) == KEY_DOWN)
	{
		actualPolygon = CUBE;
	}
	if (App->input->getKey(SDL_SCANCODE_S) == KEY_DOWN)
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
}