#include "Globals.h"
#include "Application.h"
#include "ModuleSceneMain.h"
#include "ModuleWindow.h"
#include "ModuleGameUI.h"
#include "ModuleCamera.h"
#include "CameraComponent.h"
#include "ElementGameUI.h"
#include "Shader.h"
#include "Glew/include/glew.h"
#include "MathGeoLib/src/Math/float4x4.h"
#include "ElementFactory.h"

ModuleGameUI::ModuleGameUI() 
{
	
}

ModuleGameUI::~ModuleGameUI() {}

update_status ModuleGameUI::update(float deltaTime)
{
	static ElementFactory* factory = ElementFactory::getInstance();
	elements.push_back(factory->getComponent(BUTTON, nullptr, 0, 0, 100, 100, true));
	printGameUI();
	return UPDATE_CONTINUE;
}

void ModuleGameUI::printGameUI() 
{
	GLuint program = App->sceneMain->shader->programs[App->sceneMain->shader->defaultShaders[DEFAULTSHADER]];
	glUseProgram(program);

	float4x4 identity = float4x4::identity;

	GLint modelLoc = glGetUniformLocation(program, "model_matrix");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &identity[0][0]);

	GLint viewLoc = glGetUniformLocation(program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &identity[0][0]);

	GLint projectLoc = glGetUniformLocation(program, "projection");
	glUniformMatrix4fv(projectLoc, 1, GL_FALSE, &identity[0][0]);

	float screenWidth = App->window->width;
	float screenHeight = App->window->height;

	for (vector<ElementGameUI*>::iterator it = elements.begin(); it != elements.end(); ++it)
	{
		if ((*it)->visible)
		{
			float x1 = (float)((((*it)->rect.x) / screenWidth) * 2) - 1;
			float x2 = (float)(x1 + ((*it)->rect.h) / (screenWidth / 2));
			float y1 = (float)(2 - ((((*it)->rect.y) / screenHeight) * 2) - 1);
			float y2 = (float)(2 - (y1 + ((*it)->rect.w) / (screenHeight / 2)));

			glBegin(GL_POLYGON);
				glVertex2f(x1, y1);
				glVertex2f(x1, y2);
				glVertex2f(x2, y2);
				glVertex2f(x2, y1);
			glEnd();
		}
	}
	elements.clear();
}