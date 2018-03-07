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

ModuleGameUI::ModuleGameUI() 
{
	//TO_DEBUG
	ElementGameUI* element = new ElementGameUI(BUTTON, 0, 0, 200, 200);
	elements.push_back(element);
	//END_DEBUG
}

ModuleGameUI::~ModuleGameUI() {}

update_status ModuleGameUI::update(float deltaTime)
{
	printGameUI();
	return UPDATE_CONTINUE;
}

void ModuleGameUI::printGameUI() 
{
	//TODO: Not working yet
	glOrtho(-1.0, 1.0, -1.0, 1.0, 5, 100);
	glMatrixMode(GL_MODELVIEW);

	for (vector<ElementGameUI*>::iterator it = elements.begin(); it != elements.end(); ++it)
	{
		if ((*it)->visible)
		{
			/*GLuint program = App->sceneMain->shader->programs[App->sceneMain->shader->defaultShaders[DEFAULTSHADER]];
			glUseProgram(program);

			float4x4 identity = float4x4::identity;

			GLint modelLoc = glGetUniformLocation(program, "model_matrix");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &identity[0][0]);

			GLint viewLoc = glGetUniformLocation(program, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, App->camera->getViewMatrix());

			GLint projectLoc = glGetUniformLocation(program, "projection");
			glUniformMatrix4fv(projectLoc, 1, GL_FALSE, App->camera->getProjectMatrix());

			float3 cameraPos = App->camera->currentCamera->frustum.pos;*/

			float x1 = (float)((*it)->rect.x);
			float x2 = (float)((*it)->rect.x + (*it)->rect.w);
			float y1 = (float)((*it)->rect.y);
			float y2 = (float)((*it)->rect.y + (*it)->rect.h);

			glBegin(GL_POLYGON);
				glVertex2f(x1, y1);
				glVertex2f(x2, y1);
				glVertex2f(x2, y2);
				glVertex2f(x1, y2);
			glEnd();
		}
	}
}