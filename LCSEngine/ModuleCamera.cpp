#include "ModuleCamera.h"
#include "MathGeoLib/src/Math/float3x4.h"
#include "MathGeoLib/src/Math/MathFunc.h"


ModuleCamera::ModuleCamera() {}

ModuleCamera::~ModuleCamera() {}

bool ModuleCamera::init()
{
	frustum.verticalFov = DegToRad(60.0f);
	frustum.horizontalFov = DegToRad(90.0f);
	frustum.nearPlaneDistance = 0.1;
	frustum.farPlaneDistance = 100;
	float3 position = { 0, 0, 0 };
	frustum.pos = position;
	float3 front = { 0, 0, 1 };
	frustum.front = front;
	float3 up = { 0, 1, 0 };
	frustum.up = up;
	

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
	frustum.horizontalFov = 2.0f * atan(tan((float)frustum.verticalFov / 2.0f)*(screenWidth / screenHeight));
	return true;
}



