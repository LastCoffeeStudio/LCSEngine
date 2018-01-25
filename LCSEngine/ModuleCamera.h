#ifndef __MODULECAMERA_H__
#define __MODULECAMERA_H__


#include "Module.h"
#include "MathGeoLib/src/Geometry/Frustum.h"

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
	void updatedWindowSize(int screenWidth, int screenHeight);
	void cameraZoom(float deltaTime);
	void cameraRotation(float deltaTime);

public:
	Frustum frustum;
	float cameraSpeed = 2.f;
	float rotationSpeed = 2.f;
	float zoomSpeed = 5.f;
	bool frustumCulling = false;

private:
	void moveCamera(const float deltaTime);

private:

};

#endif // __MODULECAMERA_H__