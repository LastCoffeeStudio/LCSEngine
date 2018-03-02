#ifndef __ANIMATIONCOMPONENT_H__
#define __ANIMATIONCOMPONENT_H__

#include "Component.h"
#include "MathGeoLib/src/Math/float4x4.h"

class GameObject;
class TransformComponent;

class AnimationComponent : public Component
{
public:
	AnimationComponent(GameObject* gameObject);
	~AnimationComponent();

	void drawGUI() override;
	void drawHierarchy();

public:
	GameObject* rootBone = nullptr;

private:
	void drawLine(const float4x4& idParent, const float4x4& idChild);
};

#endif //__ANIMATIONCOMPONENT_H__
