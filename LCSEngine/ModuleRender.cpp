#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleGUI.h"
#include "ModuleSceneMain.h"
#include "ModuleCamera.h"
#include "ModuleGameUI.h"
#include "CameraComponent.h"
#include "Shader.h"
#include "SDL/include/SDL.h"
#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Brofiler.h"

ModuleRender::ModuleRender() {}

// Destructor
ModuleRender::~ModuleRender() {}

// Called before render is available
bool ModuleRender::init()
{
	bool ret = true;

	SDL_GLContext context = SDL_GL_CreateContext(App->window->window);
	if (!context) {
		LOG("Couldn't create context: %s\n", SDL_GetError());
		ret = false;
	}

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		LOG("GLEW Error: %s\n", glewGetErrorString(err));
		ret = false;
	}

	//Use Vsync
	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		LOG("Warning: Unable to set VSync! SDL Error: %s", SDL_GetError());
	}

	LOG("Using Glew %s", glewGetString(GLEW_VERSION));
	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearDepth(1.0f);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glEnable(GL_TEXTURE_2D);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	return ret;
}

update_status ModuleRender::preUpdate(float deltaTime)
{
	glClearColor(App->gui->clear_color.x, App->gui->clear_color.y, App->gui->clear_color.z, App->gui->clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glMatrixMode(GL_PROJECTION);
	/*if(CONDITION) *///glLoadMatrixf(App->camera->getProjectMatrix());
	//glMatrixMode(GL_MODELVIEW);
	//glLoadMatrixf(App->camera->getViewMatrix());
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::update(float deltaTime)
{
	return UPDATE_CONTINUE;
}

update_status ModuleRender::postUpdate(float deltaTime)
{
	if (wireframe == true)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	App->sceneMain->draw();
	renderObjects();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	App->gameUI->printGameUI();
	App->gui->draw();
	//SDL_GL_SwapWindow(App->window->window);
	swap();
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::cleanUp()
{
	LOG("Destroying renderer");
	return true;
}

void ModuleRender::updatedWindowSize(int screenWidth, int screenHeight)
{
	glViewport(0, 0, screenWidth, screenHeight);
	App->window->width = screenWidth;
	App->window->height = screenHeight;
}

void ModuleRender::swap()
{
	BROFILER_CATEGORY("SwapWindow", Profiler::Color::Orchid)
		SDL_GL_SwapWindow(App->window->window);
}

void ModuleRender::renderObjects()
{
	Shader* shader = App->sceneMain->shader;
	GLuint program = 0;

	/*DEBUG*/
	static float light1 = 0.6f;
	static float light2 = 1.5f;
	static float light3 = 1.f;
	bool b = true;

	if (ImGui::Begin("light", &b, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
	{
		ImGui::PushID("light1");
		ImGui::DragFloat("lightPos1", &light1, 0.01f);
		ImGui::PopID();

		ImGui::PushID("light2");
		ImGui::DragFloat("lightPos2", &light2, 0.01f);
		ImGui::PopID();

		ImGui::PushID("light3");
		ImGui::DragFloat("lightPos3", &light3, 0.01f);
		ImGui::PopID();

		ImGui::End();
	}
	/*DEBUG*/

	for (std::multimap<GLuint, renderData>::iterator it = renderQueue.begin(); it != renderQueue.end(); ++it)
	{
		if ((*it).first != program)
		{
			program = (*it).first;
			glUseProgram(program);
		}

		GLint modelLoc = glGetUniformLocation(program, "model_matrix");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &(*it).second.id[0][0]);

		GLint viewLoc = glGetUniformLocation(program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, App->camera->getViewMatrix());

		GLint projectLoc = glGetUniformLocation(program, "projection");
		glUniformMatrix4fv(projectLoc, 1, GL_FALSE, App->camera->getProjectMatrix());

		GLint normalLoc = glGetUniformLocation(program, "normal_matrix");
		if (normalLoc != -1)
		{
			glUniformMatrix4fv(normalLoc, 1, GL_FALSE, &(*it).second.normalId[0][0]);
		}

		GLint lightPosLoc = glGetUniformLocation(program, "lightPosition");
		if (lightPosLoc != -1)
		{
			glUniform3f(lightPosLoc, light1, light2, light3);
		}

		float3 cameraPosition = App->camera->currentCamera->frustum.pos;

		GLint cameraPosLoc = glGetUniformLocation(program, "cameraPosition");
		if (cameraPosLoc != -1)
		{
			glUniform3f(cameraPosLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);
		}
		
		glUniform1i(glGetUniformLocation(program, "useText"), (*it).second.hasTexture);
		glUniform1i(glGetUniformLocation(program, "useNormal"), (*it).second.hasNormalMap);

		//Order matters!
		if ((*it).second.hasTexture)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, (*it).second.textureID);
			glUniform1i(glGetUniformLocation(program, "text"), 0);

			glBindBuffer(GL_ARRAY_BUFFER, (*it).second.textureCoordsID);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
		
			if ((*it).second.hasNormalMap)
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, (*it).second.normalMapID);
				glUniform1i(glGetUniformLocation(program, "normalMap"), 0);

				glBindBuffer(GL_ARRAY_BUFFER, (*it).second.textureCoordsID);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
			}
		}
		
		glBindBuffer(GL_ARRAY_BUFFER, (*it).second.idVertVBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		glBindBuffer(GL_ARRAY_BUFFER, (*it).second.colorID);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		glBindBuffer(GL_ARRAY_BUFFER, (*it).second.idNormalVBO);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*it).second.idIdxVAO);

		glDrawElements((*it).second.mode, (*it).second.sizeIdxVAO, GL_UNSIGNED_INT, NULL);
	}

	renderQueue.clear();
}