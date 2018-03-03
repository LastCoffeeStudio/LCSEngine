#ifndef __MODULEANIMATION_H__
#define __MODULEANIMATION_H__

#include "Module.h"
#include "MathGeoLib/src/Math/float3.h"
#include "MathGeoLib/src/Math/Quat.h"
#include <map>
#include <vector>
#include <string>

struct Frame
{
	std::string name = "";
	std::vector<float3> positions;
	std::vector<Quat> rotations;
};

struct Animation
{
	unsigned int duration = 0;
	std::vector<Frame*> frames;
};

struct AnimationInstance
{
	Animation* animation = nullptr;
	unsigned int localTime = 0;	//ms
	bool loop = true;
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
	bool getTransform(unsigned int id, const char* frameName, float3& position, Quat& rotation);

public:
	std::map<std::string, Animation*> animations;
	std::vector<AnimationInstance*> instances;
	std::vector<unsigned int> unusedIDs;
};

#endif //__MODULEANIMATION_H__