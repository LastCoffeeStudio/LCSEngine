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

public:
	Frustum frustum;
	float cameraSpeed = 2.f;

private:
	void moveCamera(const float deltaTime);

private:
<<<<<<< HEAD
	
=======
	float cameraSpeed = 2.f;
	float zoomSpeed = 5.f;
>>>>>>> a427e6b92f03f98145616146f16b7270edf613e3
};

#endif // __MODULECAMERA_H__