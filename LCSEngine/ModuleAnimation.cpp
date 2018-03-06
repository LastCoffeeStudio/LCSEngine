#include "ModuleAnimation.h"

ModuleAnimation::ModuleAnimation() {}

ModuleAnimation::~ModuleAnimation() {}

bool ModuleAnimation::cleanUp()
{
	return true;
}

bool ModuleAnimation::load(const char* name, const char* path)
{
	scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (!scene->HasAnimations())
	{
		LOG("Imported file has no animations");
		return false;
	}

	Animation* anim = new Animation();
	anim->duration = unsigned int((scene->mAnimations[0]->mDuration / scene->mAnimations[0]->mTicksPerSecond) * 1000);
	for (unsigned int i = 0; i < scene->mAnimations[0]->mNumChannels; ++i)
	{
		aiNodeAnim* nodeAnim = scene->mAnimations[0]->mChannels[i];
		Bone* bone = new Bone();
		bone->name = nodeAnim->mNodeName.C_Str();
		for (unsigned int j = 0; j < nodeAnim->mNumPositionKeys; ++j)
		{
			bone->positions.push_back(float3(nodeAnim->mPositionKeys[j].mValue.x, nodeAnim->mPositionKeys[j].mValue.y, nodeAnim->mPositionKeys[j].mValue.z));
		}
		for (unsigned int k = 0; k < nodeAnim->mNumRotationKeys; ++k)
		{
			bone->rotations.push_back(Quat(nodeAnim->mRotationKeys[k].mValue.x, nodeAnim->mRotationKeys[k].mValue.y, nodeAnim->mRotationKeys[k].mValue.z, nodeAnim->mRotationKeys[k].mValue.w));
		}

		anim->bones.push_back(bone);
	}

	animations[name] = anim;

	return true;
}

update_status ModuleAnimation::update(float deltaTime)
{
	for (std::vector<AnimationInstance*>::iterator it = instances.begin(); it != instances.end(); ++it)
	{
		if ((*it) != nullptr)
		{
			(*it)->localTime += unsigned int(deltaTime*1000);	//ms
			if ((*it)->localTime > (*it)->animation->duration)
			{
				(*it)->localTime -= (*it)->animation->duration;
			}
		}
	}
	return UPDATE_CONTINUE;
}

/*Ids goes from 1 to the number of actual instances. If ID 0 is returned, failed to play the animation*/
unsigned int ModuleAnimation::play(const char* name)
{
	unsigned int id = 0;
	std::map<std::string, Animation*>::iterator it = animations.find(name);
	if (it != animations.end())
	{
		AnimationInstance* animInstance = new AnimationInstance();
		animInstance->animation = (*it).second;

		if (unusedIDs.size() > 0)
		{
			id = unusedIDs.back();
			unusedIDs.pop_back();
			instances[id-1] = animInstance;
		}
		else
		{
			instances.push_back(animInstance);
			id = instances.size();
		}
	}
	else
	{
		LOG("Doesn't exist this animation");
	}
	
	return id;
}

void ModuleAnimation::stop(unsigned int id)
{
	if (id != 0 && id <= instances.size() && instances[id-1] != nullptr)
	{
		instances[id-1]->animation = nullptr;
		instances[id-1] = nullptr;
		unusedIDs.push_back(id);
	}
	else
	{
		LOG("No instance of animation with id: %u", id);
	}
}

bool ModuleAnimation::getTransform(unsigned int id, const char* boneName, float3& position, Quat& rotation)
{
	bool success = false;
	if (id != 0 && id <= instances.size() && instances[id - 1] != nullptr)
	{
		Animation* anim = instances[id - 1]->animation;
		for (unsigned int i = 0; i < anim->bones.size() && !success; ++i)
		{
			if (anim->bones[i]->name == boneName)
			{
				//position = calculatePosition();
				//rotation = calculateRotation();
				success = true;
			}
		}
	}
	return success;
}

float3 ModuleAnimation::calculatePosition(const float3& iniPos, const float3& endPos, float time) const
{
	float3 position = { 0.f, 0.f, 0.f };
	return position;
}

Quat ModuleAnimation::calculateRotation(const Quat& iniRot, const Quat& endRot, float time) const
{
	Quat rotation = { 0.f, 0.f, 0.f, 0.f };
	return rotation;
}