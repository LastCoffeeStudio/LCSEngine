#ifndef __CAMERACOMPONENT_H__
#define __CAMERACOMPONENT_H__

#include "Component.h"
#include "MathGeoLib/src/Geometry/Frustum.h"

class CameraComponent : public Component
{
public:
	CameraComponent(GameObject* gameObject, bool isEnable = true, bool isUnique = false);
	~CameraComponent();

	float* getViewMatrix();
	float* getProjectMatrix();
	float* getViewMatrix4x4();
	float4x4 getViewProjectMatrix();
	void drawGUI() override;

public:
	Frustum frustum;
	bool frustumCulling = false;
};

#endif //__CAMERACOMPONENT_H__