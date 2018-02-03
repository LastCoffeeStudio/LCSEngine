#include "Application.h"
#include "ModuleCamera.h"
#include "MathGeoLib/src/Math/float3x4.h"
#include "MathGeoLib/src/Math/MathFunc.h"
#include <SDL.h>
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "MathGeoLib/src/Math/Quat.h"


ModuleCamera::ModuleCamera() {}


ModuleCamera::~ModuleCamera() {}


bool ModuleCamera::init()
{

	frustum.type = PerspectiveFrustum;
	frustum.verticalFov = DegToRad(60.0f);
	frustum.horizontalFov = DegToRad(36.0f);
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.f;
	frustum.pos = float3::unitY;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	updatedWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return  true;
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


void ModuleCamera::updatedWindowSize(int screenWidth, int screenHeight)
{
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov / 2.0f)*((float)screenWidth / (float)screenHeight));
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
		displacement += frustum.front;
		movement = true;
	}
	if (App->input->getKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		displacement -= frustum.front;
		movement = true;
	}
	if (App->input->getKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->getMouseMotion().x < 0 && App->input->getMouseButtonDown(SDL_BUTTON_MIDDLE))
	{
		displacement += frustum.WorldRight();
		movement = true;
	}
	if (App->input->getKey(SDL_SCANCODE_A) == KEY_REPEAT || App->input->getMouseMotion().x > 0 && App->input->getMouseButtonDown(SDL_BUTTON_MIDDLE))
	{
		displacement -= frustum.WorldRight();
		movement = true;
	}
	if (App->input->getMouseMotion().y > 0 && App->input->getMouseButtonDown(SDL_BUTTON_MIDDLE))
	{
		displacement += frustum.up;
		movement = true;
	}
	if (App->input->getMouseMotion().y < 0 && App->input->getMouseButtonDown(SDL_BUTTON_MIDDLE))
	{
		displacement -= frustum.up;
		movement = true;
	}
	if (movement = true)
	{
		frustum.Translate(displacement*speed);
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
		displacement += frustum.front;
		doZoom = true;
	}
	if (App->input->getKey(SDL_SCANCODE_Z) == KEY_REPEAT || App->input->mouse_wheel > 0)
	{
		displacement -= frustum.front;
		doZoom = true;
	}
	if(doZoom == true)
	{
		frustum.Translate(displacement*speed);
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
		if (frustum.front.y < 0.999f)
		{
			rotation = rotation.Mul(Quat::RotateAxisAngle(frustum.WorldRight(), -speed));
			rotated = true;
		}
	}
	if (App->input->getKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || App->input->getMouseMotion().y < 0 && App->input->getMouseButtonDown(SDL_BUTTON_RIGHT))
	{
		if (frustum.front.y > -0.999f)
		{
			rotation = rotation.Mul(Quat::RotateAxisAngle(frustum.WorldRight(), speed));
			rotated = true;
		}
	}
	if (rotated == true)
	{
		frustum.up = rotation.Mul(frustum.up);
		frustum.front = rotation.Mul(frustum.front);
		frustum.up.Normalize();
		frustum.front.Normalize();
	}
}
