#include "Application.h"
#include "ModuleCamera.h"
#include "MathGeoLib/src/Math/float3x4.h"
#include <glew.h>
#include "ModuleInput.h"

ModuleCamera::ModuleCamera() {}

ModuleCamera::~ModuleCamera() {}

bool ModuleCamera::init()
{
	frustum.verticalFov = 0.94248f;
	frustum.horizontalFov = 1.41372f;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.f;
	float3 position = { 0.f, 0.f, 0.f };
	frustum.pos = position;
	float3 front = { 0.f, 0.f, -1.f };
	frustum.front = front;
	float3 up = { 0.f, 1.f, 0.f };
	frustum.up = up;

	return  true;
}

update_status ModuleCamera::update(const float deltaTime)
{
	moveCamera(deltaTime);
	return UPDATE_CONTINUE;
}

bool ModuleCamera::cleanUp()
{
	return  true;
}

float* ModuleCamera::getViewMatrix()
{
	 return ((float4x4)frustum.ViewMatrix()).Transposed().v[0];
}

float* ModuleCamera::getProjectMatrix()
{
	return frustum.ProjectionMatrix().Transposed().v[0];
}

void ModuleCamera::moveCamera(const float deltaTime)
{
	float speed = cameraSpeed*deltaTime;
	float3 displacement = { 0.f, 0.f, 0.f };

	if (App->input->getKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	{
		speed *= 2.f;
	}

	if (App->input->getKey(SDL_SCANCODE_Q) == KEY_REPEAT)
	{
		displacement += { 0.f, 1.f, 0.f };
	}

	if (App->input->getKey(SDL_SCANCODE_E) == KEY_REPEAT)
	{
		displacement -= { 0.f, 1.f, 0.f };
	}

	if (App->input->getKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		displacement += frustum.front;
	}

	if (App->input->getKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		displacement -= frustum.front;
	}

	if (App->input->getKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		displacement += frustum.WorldRight();
	}

	if (App->input->getKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		displacement -= frustum.WorldRight();
	}

	frustum.Translate(displacement*speed);
}