#ifndef __TRANSFORMCOMPONENT_H__
#define __TRANSFORMCOMPONENT_H__

#include "Component.h"
#include "MathGeoLib/src/Math/float3.h"
#include "MathGeoLib/src/Math/float4x4.h"
#include "MathGeoLib/src/Math/Quat.h"
#include "assimp/include/scene.h"

class TransformComponent : public Component
{
public:
	TransformComponent(GameObject* gameObject);
	~TransformComponent();

	void drawGUI() override;

public:
	float3 position = { 0.0f, 0.0f, 0.0f };
	float3 scale = { 1.0f, 1.0f, 1.0f };
	float3 rotation = { 0.0f, 0.0f, 0.0f };
	Quat rotationQad = Quat({ 0.0f, 0.0f, 1.0f }, 0.0f);
	float4x4 transform;
	void setTransform(const aiMatrix4x4 & matAiScene);
	void updateTransform();

private:

public:
	float4x4 matrixTranslate;
	float4x4 matrixScale;
	float4x4 matrixRotate;

	void updateTranslate();
	void updateScale();
	void updateRotate();
};

#endif //__TRANSFORMCOMPONENT_H__