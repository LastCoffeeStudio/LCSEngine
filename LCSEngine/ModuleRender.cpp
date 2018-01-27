#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleGUI.h"
#include "ModuleInput.h"
#include "ModuleSceneMain.h"
#include "SDL/include/SDL.h"
#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include "ModuleCamera.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include <string>

ModuleRender::ModuleRender() {}

// Destructor
ModuleRender::~ModuleRender() {}

// Called before render is available
bool ModuleRender::init()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	bool ret = true;

	SDL_GLContext context = SDL_GL_CreateContext(App->window->window);
	if (!context) {
		printf("Couldn't create context: %s\n", SDL_GetError());
		ret = false;
	}

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		printf("GLEW Error: %s\n", glewGetErrorString(err));
		ret = false;
	}

	//Use Vsync
	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
	}

	printf("Using Glew %s\n", glewGetString(GLEW_VERSION));
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("OpenGL version supported %s\n", glGetString(GL_VERSION));
	printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearDepth(1.0f);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	return ret;
}

update_status ModuleRender::preUpdate(float deltaTime)
{
	glClearColor(App->gui->clear_color.x, App->gui->clear_color.y, App->gui->clear_color.z, App->gui->clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	/*if(CONDITION) */glLoadMatrixf(App->camera->getProjectMatrix());
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->getViewMatrix());
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
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	App->gui->draw();
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::cleanUp()
{
	LOG("Destroying renderer");

	//Destroy window
	

	return true;
}


void ModuleRender::updatedWindowSize(int screenWidth, int screenHeight)
{
	glViewport(0, 0, screenWidth, screenHeight);
}
