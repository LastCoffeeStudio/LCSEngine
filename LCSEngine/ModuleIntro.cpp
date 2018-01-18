#include "Globals.h"
#include "Application.h"
#include "ModuleIntro.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleJson.h"
#include "Glew/include/glew.h"

#include <iostream>
using namespace std;

ModuleIntro::ModuleIntro() {}


ModuleIntro::~ModuleIntro() {}

bool ModuleIntro::Start()
{
	return true;
}

update_status ModuleIntro::Update(const float deltaTime)
{
	glBegin(GL_TRIANGLES);
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(-1.0f, -0.5f, -4.0f); // lower left vertex
	glVertex3f(1.0f, -0.5f, -4.0f); // lower right vertex
	glVertex3f(0.0f, 0.5f, -4.0f); // upper vertex
	glEnd();
	return UPDATE_CONTINUE;
}
