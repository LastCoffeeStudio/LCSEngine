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
	std::string animationName = "";
	std::string currentAnimationName = "";
	unsigned int idAnim = 0;
	float blendTime = 0.f;

private:
	void drawLine(const float4x4& idParent, const float4x4& idChild);
	void playAnimation();
	void stopAnimation();
	void blendAnimation();
};

#endif //__ANIMATIONCOMPONENT_H__
