#ifndef __MODULECAMERA_H__
#define __MODULECAMERA_H__


#include "Module.h"
#include "MathGeoLib/src/Geometry/Frustum.h"
#include "MathGeoLib/src/Math/float3x4.h"

class ModuleCamera : public Module
{
public:

	ModuleCamera();
	~ModuleCamera();

	bool init();
	update_status update(const float deltaTime);
	bool cleanUp();
	float* getViewMatrix();
	float* getProjectMatrix();

public:
	Frustum frustum;

private:
	void moveCamera(const float deltaTime);

private:
	float cameraSpeed = 2.f;
};

#endif // __MODULECAMERA_H__