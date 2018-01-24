#include "Application.h"
#include "ModuleCamera.h"
#include "MathGeoLib/src/Math/float3x4.h"
#include "MathGeoLib/src/Math/MathFunc.h"
#include <SDL.h>
#include "ModuleInput.h"


ModuleCamera::ModuleCamera() {}

ModuleCamera::~ModuleCamera() {}



bool ModuleCamera::init()
{

	frustum.type = PerspectiveFrustum;
	frustum.verticalFov = DegToRad(60.0f);
	frustum.horizontalFov = DegToRad(36.0f);
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.f;
	float3 position = { 0.0f, 1.0f, 0.0f };
	frustum.pos = position;
	float3 front = { 0.0f, 0.0f, -1.0f };
	frustum.front = front;
	float3 up = { 0.0f, 1.0f, 0.0f };
	frustum.up = up;
	updatedWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

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



bool ModuleCamera::updatedWindowSize(int screenWidth, int screenHeight)
{
	frustum.horizontalFov = 2.0f * atan(tan(frustum.verticalFov / 2.0f)*(screenWidth / screenHeight));
	return true;
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
