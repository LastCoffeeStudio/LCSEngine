#ifndef __MODULECAMERA_H__
#define __MODULECAMERA_H__

#include "Module.h"
#include "MathGeoLib/src/Geometry/Frustum.h"

class CameraComponent;

class ModuleCamera : public Module
{
public:

	ModuleCamera();
	~ModuleCamera();

	bool init();
	update_status update(float deltaTime);
	bool cleanUp();
	float* getViewMatrix();
	float* getProjectMatrix();
	void updatedWindowSize(int screenWidth, int screenHeight);
	void cameraZoom(float deltaTime);
	void cameraRotation(float deltaTime);

public:
	
	float cameraSpeed = 6.f;
	float rotationSpeed = 2.f;
	float zoomSpeed = 20.f;

	CameraComponent* currentCamera = nullptr;

private:
	void moveCamera(float deltaTime);

private:

};

#endif // __MODULECAMERA_H__