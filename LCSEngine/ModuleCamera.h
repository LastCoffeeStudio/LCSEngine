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
	bool cleanUp();
	float* getViewMatrix();
	float* getProjectMatrix();
	bool updatedWindowSize(int screenWidth, int screenHeight);

public:

	Frustum frustum;

private:

private:


};

#endif // __MODULECAMERA_H__