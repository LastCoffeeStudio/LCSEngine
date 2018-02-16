#ifndef __MODULECAMERA_H__
#define __MODULECAMERA_H__

#include "Module.h"
#include "MathGeoLib/src/Geometry/Frustum.h"
#include "MathGeoLib/src/Geometry/LineSegment.h"

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
	float* getViewMatrix4x4();
	float* getProjectMatrix();
	float4x4 getViewProjectMatrix();
	void updatedWindowSize(int screenWidth, int screenHeight);
	void cameraZoom(float deltaTime);
	void cameraRotation(float deltaTime);
	void onClickEvent();

public:
	
	float cameraSpeed = 6.f;
	float rotationSpeed = 2.f;
	float zoomSpeed = 20.f;

	CameraComponent* currentCamera = nullptr;

private:
	void moveCamera(float deltaTime);

private:
	LineSegment l;
};

#endif // __MODULECAMERA_H__