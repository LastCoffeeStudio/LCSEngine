#ifndef __TRANSFORMCOMPONENT_H__
#define __TRANSFORMCOMPONENT_H__

#include "Component.h"
#include "MathGeoLib/src/MathGeoLib.h"

class TransformComponent : public Component
{
public:
	TransformComponent(GameObject* gameObject, std::string name, bool isEnable = false, bool isUnique = false);
	~TransformComponent();

	void drawGUI() override;

public:
	float3 position = {0.0f, 0.0f, 0.0f};
	float3 scale = { 0.0f, 0.0f, 0.0f };
	float3 rotation = { 0.0f, 0.0f, 0.0f };
	Quat rotationQad = Quat({0.0f, 0.0f, 1.0f}, 0.0f);
};

#endif