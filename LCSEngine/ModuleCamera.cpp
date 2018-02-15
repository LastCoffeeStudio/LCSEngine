#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "CameraComponent.h"
#include "MathGeoLib/src/Math/float3x4.h"
#include "MathGeoLib/src/Math/MathFunc.h"
#include "MathGeoLib/src/Math/Quat.h"
#include "Brofiler.h"
#include <SDL.h>

#include "Glew/include/glew.h"
#include "Shader.h"
#include "ModuleSceneMain.h"
#include "PhysRaycast.h"

ModuleCamera::ModuleCamera() {}

ModuleCamera::~ModuleCamera() {}

bool ModuleCamera::init()
{
	currentCamera = new CameraComponent(nullptr);
	updatedWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	return true;
}

update_status ModuleCamera::update(float deltaTime)
{
	BROFILER_CATEGORY("ModuleCamera", Profiler::Color::Orchid)
	moveCamera(deltaTime);
	cameraZoom(deltaTime);
	cameraRotation(deltaTime);
	onClickEvent();
	return UPDATE_CONTINUE;
}

bool ModuleCamera::cleanUp()
{
	return true;
}

float* ModuleCamera::getViewMatrix()
{
	return currentCamera->getViewMatrix();
}

float* ModuleCamera::getProjectMatrix()
{
	return currentCamera->getProjectMatrix();
}

float* ModuleCamera::getViewMatrix4x4()
{
	return currentCamera->getViewMatrix4x4();
}

float4x4 ModuleCamera::getViewProjectMatrix()
{
	return currentCamera->getViewProjectMatrix();
}

void ModuleCamera::updatedWindowSize(int screenWidth, int screenHeight)
{
	currentCamera->frustum.horizontalFov = 2.0f * atanf(tanf(currentCamera->frustum.verticalFov / 2.0f)*((float)screenWidth / (float)screenHeight));
}

void ModuleCamera::moveCamera(float deltaTime)
{
	float speed = cameraSpeed * deltaTime;
	float3 displacement = { 0.f, 0.f, 0.f };
	bool movement = false;
	if (App->input->getKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	{
		speed *= 2.f;
	}
	if (App->input->getKey(SDL_SCANCODE_Q) == KEY_REPEAT)
	{
		displacement += { 0.f, 1.f, 0.f };
		movement = true;
	}
	if (App->input->getKey(SDL_SCANCODE_E) == KEY_REPEAT)
	{
		displacement -= { 0.f, 1.f, 0.f };
		movement = true;
	}
	if (App->input->getKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		displacement += currentCamera->frustum.front;
		movement = true;
	}
	if (App->input->getKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		displacement -= currentCamera->frustum.front;
		movement = true;
	}
	if (App->input->getKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->getMouseMotion().x < 0 && App->input->getMouseButtonDown(SDL_BUTTON_MIDDLE))
	{
		displacement += currentCamera->frustum.WorldRight();
		movement = true;
	}
	if (App->input->getKey(SDL_SCANCODE_A) == KEY_REPEAT || App->input->getMouseMotion().x > 0 && App->input->getMouseButtonDown(SDL_BUTTON_MIDDLE))
	{
		displacement -= currentCamera->frustum.WorldRight();
		movement = true;
	}
	if (App->input->getMouseMotion().y > 0 && App->input->getMouseButtonDown(SDL_BUTTON_MIDDLE))
	{
		displacement += currentCamera->frustum.up;
		movement = true;
	}
	if (App->input->getMouseMotion().y < 0 && App->input->getMouseButtonDown(SDL_BUTTON_MIDDLE))
	{
		displacement -= currentCamera->frustum.up;
		movement = true;
	}
	if (movement = true)
	{
		currentCamera->frustum.Translate(displacement*speed);
	}
}

void ModuleCamera::cameraZoom(float deltaTime)
{
	float speed = zoomSpeed * deltaTime;
	float3 displacement = { 0.f, 0.f, 0.f };
	bool doZoom = false;
	if (App->input->getKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	{
		speed *= 2.f;
	}
	if (App->input->getKey(SDL_SCANCODE_X) == KEY_REPEAT || App->input->mouse_wheel < 0)
	{
		displacement += currentCamera->frustum.front;
		doZoom = true;
	}
	if (App->input->getKey(SDL_SCANCODE_Z) == KEY_REPEAT || App->input->mouse_wheel > 0)
	{
		displacement -= currentCamera->frustum.front;
		doZoom = true;
	}
	if (doZoom == true)
	{
		currentCamera->frustum.Translate(displacement*speed);
	}
}

void ModuleCamera::cameraRotation(float deltaTime)
{
	float speed = rotationSpeed * deltaTime;
	Quat rotation = Quat::identity;
	bool rotated = false;
	if (App->input->getKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	{
		speed *= 2.f;
	}
	if (App->input->getKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input->getMouseMotion().x > 0 && App->input->getMouseButtonDown(SDL_BUTTON_RIGHT))
	{
		rotation = rotation.Mul(Quat::RotateY(-speed));
		rotated = true;
	}
	if (App->input->getKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input->getMouseMotion().x < 0 && App->input->getMouseButtonDown(SDL_BUTTON_RIGHT))
	{
		rotation = rotation.Mul(Quat::RotateY(speed));
		rotated = true;
	}
	if (App->input->getKey(SDL_SCANCODE_UP) == KEY_REPEAT || App->input->getMouseMotion().y > 0 && App->input->getMouseButtonDown(SDL_BUTTON_RIGHT))
	{
		if (currentCamera->frustum.front.y < 0.999f)
		{
			rotation = rotation.Mul(Quat::RotateAxisAngle(currentCamera->frustum.WorldRight(), -speed));
			rotated = true;
		}
	}
	if (App->input->getKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || App->input->getMouseMotion().y < 0 && App->input->getMouseButtonDown(SDL_BUTTON_RIGHT))
	{
		if (currentCamera->frustum.front.y > -0.999f)
		{
			rotation = rotation.Mul(Quat::RotateAxisAngle(currentCamera->frustum.WorldRight(), speed));
			rotated = true;
		}
	}
	if (rotated == true)
	{
		currentCamera->frustum.up = rotation.Mul(currentCamera->frustum.up);
		currentCamera->frustum.front = rotation.Mul(currentCamera->frustum.front);
		currentCamera->frustum.up.Normalize();
		currentCamera->frustum.front.Normalize();
	}
}

void ModuleCamera::onClickEvent()
{
	if (App->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		iPoint mousePos = App->input->getMousePosition();
		float2 fMousePos = currentCamera->frustum.ScreenToViewportSpace((float)mousePos.x, (float)mousePos.y, App->window->width, App->window->height);
		l = currentCamera->frustum.UnProjectLineSegment(fMousePos.x, fMousePos.y);
		PhysRaycast* ray = new PhysRaycast();
		if (ray->castRay(l, Distance(l.a, l.b)))
		{
			App->sceneMain->currentObject = ray->gameObject;
		}
	}
	/*DEBUG*/
	drawLine(l.a, l.b);
	/*END DEBUG*/
}

void ModuleCamera::drawLine(float3 origin, float3 end)
{
	float4x4 identity = float4x4::identity;
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	GLint modelLoc = glGetUniformLocation(App->sceneMain->shader->shaderProgram, "model_matrix");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &identity[0][0]);

	GLint viewLoc = glGetUniformLocation(App->sceneMain->shader->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, App->camera->getViewMatrix());

	GLint projectLoc = glGetUniformLocation(App->sceneMain->shader->shaderProgram, "projection");
	glUniformMatrix4fv(projectLoc, 1, GL_FALSE, App->camera->getProjectMatrix());

	glBegin(GL_LINES);

	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(end.x, end.y, end.z);

	glEnd();

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindTexture(GL_TEXTURE_2D, 0);
}
