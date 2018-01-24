#include "ModuleCamera.h"
#include "MathGeoLib/src/Math/float3x4.h"
#include "MathGeoLib/src/Math/MathFunc.h"


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



