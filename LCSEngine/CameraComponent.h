#ifndef __CAMERACOMPONENT_H__
#define __CAMERACOMPONENT_H__

#include "Component.h"
#include "MathGeoLib/src/Geometry/Frustum.h"

class CameraComponent : public Component
{
public:
	CameraComponent();
	~CameraComponent();

	float* getViewMatrix();
	float* getProjectMatrix();
	void drawGUI() override;

public:
	Frustum frustum;
	bool frustumCulling = false;
};

#endif
