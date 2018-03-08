#ifndef __MODULEANIMATION_H__
#define __MODULEANIMATION_H__

#include "Module.h"
#include "MathGeoLib/src/Math/float3.h"
#include "MathGeoLib/src/Math/Quat.h"
#include "assimp/include/scene.h"
#include "assimp/include/cimport.h"
#include "assimp/include/postprocess.h" 
#include <map>
#include <vector>
#include <string>

struct Joint
{
	std::string name = "";
	std::vector<float3> positions;
	std::vector<Quat> rotations;
};

struct Animation
{
	unsigned int duration = 0;
	std::vector<Joint*> joints;
};

struct AnimationInstance
{
	Animation* animation = nullptr;
	unsigned int localTime = 0;	//ms
	bool loop = true;

	AnimationInstance* blendingAnim = nullptr;
	unsigned int blendDuration = 0;
	unsigned int blendTime = 0;
};

class ModuleAnimation : public Module
{
public:
	ModuleAnimation();
	~ModuleAnimation();

	bool cleanUp();

	bool load(const char* name, const char* path);
	update_status update(float deltaTime);
	unsigned int play(const char* name);
	void stop(unsigned int id);
	bool getTransform(unsigned int id, const char* boneName, float3& position, Quat& rotation);
	bool getTransformBlend(const AnimationInstance* anim, const char* boneName, float3& position, Quat& rotation);
	void blendTo(unsigned int id, const char* name, unsigned int blendTime);

	bool isBlending(unsigned int id);

public:
	std::map<std::string, Animation*> animations;
	std::vector<AnimationInstance*> instances;
	std::vector<unsigned int> unusedIDs;
	const aiScene* scene = 0;

private:
	float3 linearInterpolationPosition(const float3& iniPos, const float3& endPos, float time) const;
	Quat linearInterpolationRotation(const Quat& iniRot, const Quat& endRot, float time) const;
};

#endif //__MODULEANIMATION_H__