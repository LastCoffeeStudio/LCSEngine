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
#include "UIButton.h"
#include "UIEditText.h"
#include "Shader.h"
#include "ModuleType.h"
#include "Glew/include/glew.h"
#include "MathGeoLib/src/Math/float4x4.h"
#include "ElementFactory.h"
#include "ModuleTextures.h"

ModuleGameUI::ModuleGameUI() {}

ModuleGameUI::~ModuleGameUI() {}

void ModuleGameUI::printGameUI() 
{
	glDisable(GL_DEPTH_TEST);
	
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

		if ((*it)->visible)
		{
			switch ((*it)->type)
			{
				case IMAGE:
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
					break;
				}
				case BUTTON:
				{
					UIImage* image = ((UIButton*)(*it))->activeImage;
					glUniform1i(glGetUniformLocation(program, "useText"), image->hasTexture);

					//Order matters!
					if (image->hasTexture)
					{
						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, image->texID);
						glUniform1i(glGetUniformLocation(program, "text"), 0);

						glBindBuffer(GL_ARRAY_BUFFER, image->idTexCoords);
						glEnableVertexAttribArray(1);
						glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
					}

					glBindBuffer(GL_ARRAY_BUFFER, image->idVertVBO);
					glEnableVertexAttribArray(0);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

					glBindBuffer(GL_ARRAY_BUFFER, image->idColors);
					glEnableVertexAttribArray(2);
					glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, image->idIdxVAO);

					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

					UILabel* label = (UILabel*)((UIButton*)(*it))->label;
					UIImage* imageLabel = (UIImage*)label->textImage;

					glUniform1i(glGetUniformLocation(program, "useText"), true);

					if (label->fontData != nullptr)
					{
						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, label->fontData->idTexture);
						glUniform1i(glGetUniformLocation(program, "text"), 0);

						glBindBuffer(GL_ARRAY_BUFFER, imageLabel->idTexCoords);
						glEnableVertexAttribArray(1);
						glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
					}

					glBindBuffer(GL_ARRAY_BUFFER, imageLabel->idVertVBO);
					glEnableVertexAttribArray(0);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

					glBindBuffer(GL_ARRAY_BUFFER, imageLabel->idColors);
					glEnableVertexAttribArray(2);
					glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, imageLabel->idIdxVAO);

					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
					break;
				}
				case EDITTEXT:
				{
					UIImage* background = ((UIEditText*)(*it))->background;
					glUniform1i(glGetUniformLocation(program, "useText"), background->hasTexture);

					//Order matters!
					if (background->hasTexture)
					{
						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, background->texID);
						glUniform1i(glGetUniformLocation(program, "text"), 0);

						glBindBuffer(GL_ARRAY_BUFFER, background->idTexCoords);
						glEnableVertexAttribArray(1);
						glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
					}

					glBindBuffer(GL_ARRAY_BUFFER, background->idVertVBO);
					glEnableVertexAttribArray(0);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

					glBindBuffer(GL_ARRAY_BUFFER, background->idColors);
					glEnableVertexAttribArray(2);
					glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, background->idIdxVAO);

					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

					UILabel* label = (UILabel*)((UIEditText*)(*it))->label;
					UIImage* imageLabel = (UIImage*)label->textImage;

					glUniform1i(glGetUniformLocation(program, "useText"), true);

					if (label->fontData != nullptr)
					{
						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, label->fontData->idTexture);
						glUniform1i(glGetUniformLocation(program, "text"), 0);

						glBindBuffer(GL_ARRAY_BUFFER, imageLabel->idTexCoords);
						glEnableVertexAttribArray(1);
						glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
					}

					glBindBuffer(GL_ARRAY_BUFFER, imageLabel->idVertVBO);
					glEnableVertexAttribArray(0);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

					glBindBuffer(GL_ARRAY_BUFFER, imageLabel->idColors);
					glEnableVertexAttribArray(2);
					glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, imageLabel->idIdxVAO);

					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
					break;
				}
				case LABEL:
				{
					UILabel* label = (UILabel*)(*it);
					UIImage* image = (UIImage*)label->textImage;

					glUniform1i(glGetUniformLocation(program, "useText"), true);

					if (label->fontData != nullptr)
					{
						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, label->fontData->idTexture);
						glUniform1i(glGetUniformLocation(program, "text"), 0);

						glBindBuffer(GL_ARRAY_BUFFER, image->idTexCoords);
						glEnableVertexAttribArray(1);
						glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
					}

					glBindBuffer(GL_ARRAY_BUFFER, image->idVertVBO);
					glEnableVertexAttribArray(0);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

					glBindBuffer(GL_ARRAY_BUFFER, image->idColors);
					glEnableVertexAttribArray(2);
					glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, image->idIdxVAO);

					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
				}
				default:
					break;
			}
		}
	}
	glEnable(GL_DEPTH_TEST);
	elements.clear();
}