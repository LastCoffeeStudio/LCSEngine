#include "Application.h"
#include "ModuleCamera.h"
#include "MathGeoLib/src/Math/float3x4.h"
#include "MathGeoLib/src/Math/MathFunc.h"
#include <SDL.h>
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "CameraComponent.h"
#include "MathGeoLib/src/Math/Quat.h"


ModuleCamera::ModuleCamera() {}


ModuleCamera::~ModuleCamera() {}


bool ModuleCamera::init()
{
	currentCamera = new CameraComponent();
	updatedWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	return true;
}


update_status ModuleCamera::update(float deltaTime)
{
	moveCamera(deltaTime);
	cameraZoom(deltaTime);
	cameraRotation(deltaTime);
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


void ModuleCamera::updatedWindowSize(int screenWidth, int screenHeight)
{
	currentCamera->frustum.horizontalFov = 2.0f * atanf(tanf(currentCamera->frustum.verticalFov / 2.0f)*((float)screenWidth / (float)screenHeight));
}


void ModuleCamera::moveCamera(float deltaTime)
{
	float speed = cameraSpeed*deltaTime;
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
	if (App->input->getKey(SDL_SCANCODE_W) == KEY_REPEAT )
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
	float speed = zoomSpeed*deltaTime;
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
	if(doZoom == true)
	{
		currentCamera->frustum.Translate(displacement*speed);
	}
}

void ModuleCamera::cameraRotation(float deltaTime)
{
	float speed = rotationSpeed*deltaTime;
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
