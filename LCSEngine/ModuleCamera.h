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
	bool cleanUp();
	float* getViewMatrix();
	float* getProjectMatrix();

public:

	Frustum frustum;

private:

private:


};

#endif // __MODULECAMERA_H__