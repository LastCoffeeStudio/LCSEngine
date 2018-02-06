#ifndef __TRANSFORMCOMPONENT_H__
#define __TRANSFORMCOMPONENT_H__

#include "Component.h"
#include "MathGeoLib/src/MathGeoLib.h"

class TransformComponent : public Component
{
public:
	TransformComponent(GameObject* gameObject, bool isUnique = true);
	~TransformComponent();

	void drawGUI() override;

public:
	float3 position = { 0.0f, 0.0f, 0.0f };
	float3 scale = { 1.0f, 1.0f, 1.0f };
	float3 rotation = { 0.0f, 0.0f, 0.0f };
	Quat rotationQad = Quat({ 0.0f, 0.0f, 1.0f }, 0.0f);
	float4x4 transform;

private:
	void updateTransform();

private:
	float4x4 matrixTranslate;
	float4x4 matrixScale;
	float4x4 matrixRotate;
};

#endif