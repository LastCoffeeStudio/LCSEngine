#ifndef __ANIMATIONCOMPONENT_H__
#define __ANIMATIONCOMPONENT_H__

#include "Component.h"
#include "MathGeoLib/src/Math/float4x4.h"
#include <map>

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
	std::map<std::string, GameObject*> joints;
	std::string animationName = "";
	std::string currentAnimationName = "";
	char rootNodeBones[128] = "";
	unsigned int idAnim = 0;
	float blendTime = 0.f;

private:
	void drawLine(const float4x4& idParent, const float4x4& idChild);
	void playAnimation();
	void stopAnimation();
	void blendAnimation();
	void setRootNodeBones();
	void fillJoints();
};

#endif //__ANIMATIONCOMPONENT_H__
