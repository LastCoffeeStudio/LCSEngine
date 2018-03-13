#include "Globals.h"
#include "Application.h"
#include "ModuleSceneMain.h"
#include "ModuleWindow.h"
#include "ModuleGameUI.h"
#include "ModuleCamera.h"
#include "CameraComponent.h"
#include "ElementGameUI.h"
#include "UIImage.h"
#include "UILabel.h"
#include "Shader.h"
#include "ModuleType.h"
#include "Glew/include/glew.h"
#include "MathGeoLib/src/Math/float4x4.h"
#include "ElementFactory.h"

ModuleGameUI::ModuleGameUI() {}

ModuleGameUI::~ModuleGameUI() {}

update_status ModuleGameUI::update(float deltaTime)
{
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

	for (vector<ElementGameUI*>::iterator it = elements.begin(); it != elements.end(); ++it)
	{
		if ((*it)->visible && (*it)->type == IMAGE)
		{
			glUniform1i(glGetUniformLocation(program, "useText"), ((UIImage*)(*it))->hasTexture);

			//Order matters!
			if (((UIImage*)(*it))->hasTexture)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, ((UIImage*)(*it))->texID);
				glUniform1i(glGetUniformLocation(program, "text"), 0);

				glBindBuffer(GL_ARRAY_BUFFER, ((UIImage*)(*it))->idTexCoords);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
			}

			glBindBuffer(GL_ARRAY_BUFFER, ((UIImage*)(*it))->idVertVBO);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

			glBindBuffer(GL_ARRAY_BUFFER, ((UIImage*)(*it))->idColors);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ((UIImage*)(*it))->idIdxVAO);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else if ((*it)->visible && (*it)->type == LABEL)
		{
			UILabel* label = (UILabel*)(*it);
			UIImage* image = (UIImage*)label->textImage;

			glUniform1i(glGetUniformLocation(program, "useText"), true);

			if (label->fontData != nullptr)
			{
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, label->fontData->idTexture);
					glUniform1i(glGetUniformLocation(program, "text"), 0);

					glBindBuffer(GL_ARRAY_BUFFER, image->idTexCoords);
					glEnableVertexAttribArray(1);
					glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
				}
			}
			
			glBindBuffer(GL_ARRAY_BUFFER, image->idVertVBO);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

			glBindBuffer(GL_ARRAY_BUFFER, image->idColors);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, image->idIdxVAO);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	//elements.clear();
}