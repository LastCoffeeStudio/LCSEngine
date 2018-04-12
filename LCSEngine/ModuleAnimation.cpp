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
	if (scene == nullptr || !scene->HasAnimations())
	{
		LOG("File invalid or has no animations");
		return false;
	}

	Animation* anim = new Animation();
	anim->duration = unsigned int((scene->mAnimations[0]->mDuration / scene->mAnimations[0]->mTicksPerSecond) * 1000);
	for (unsigned int i = 0; i < scene->mAnimations[0]->mNumChannels; ++i)
	{
		aiNodeAnim* nodeAnim = scene->mAnimations[0]->mChannels[i];
		Joint* joint = new Joint();
		joint->name = nodeAnim->mNodeName.C_Str();
		for (unsigned int j = 0; j < nodeAnim->mNumPositionKeys; ++j)
		{
			joint->positions.push_back(float3(nodeAnim->mPositionKeys[j].mValue.x, nodeAnim->mPositionKeys[j].mValue.y, nodeAnim->mPositionKeys[j].mValue.z));
		}
		for (unsigned int k = 0; k < nodeAnim->mNumRotationKeys; ++k)
		{
			joint->rotations.push_back(Quat(nodeAnim->mRotationKeys[k].mValue.x, nodeAnim->mRotationKeys[k].mValue.y, nodeAnim->mRotationKeys[k].mValue.z, nodeAnim->mRotationKeys[k].mValue.w));
		}

		anim->joints.push_back(joint);
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
			(*it)->localTime += unsigned int(deltaTime * 2000);	//ms	//Speed time at 2x!!
			while ((*it)->localTime > (*it)->animation->duration)
			{
				(*it)->localTime -= (*it)->animation->duration;
			}

			if ((*it)->blendingAnim != nullptr)
			{
				(*it)->blendingAnim->localTime += unsigned int(deltaTime*1000);
				while ((*it)->blendingAnim->localTime > (*it)->blendingAnim->animation->duration)
				{
					(*it)->blendingAnim->localTime -= (*it)->blendingAnim->animation->duration;
				}
				(*it)->blendTime += unsigned int(deltaTime * 1000);
				if ((*it)->blendTime > (*it)->blendDuration)
				{
					(*it)->blendTime = 0;
					AnimationInstance* animBlend = (*it)->blendingAnim;
					delete *it;
					(*it) = animBlend;
				}
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
	if (id != 0 && id <= instances.size() && instances[id - 1] != nullptr)
	{
		instances[id - 1]->animation = nullptr;
		instances[id - 1]->blendingAnim = nullptr;
		instances[id - 1] = nullptr;
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
		for (unsigned int i = 0; i < anim->joints.size() && !success; ++i)
		{
			if (anim->joints[i]->name == boneName)
			{
				/*float3 iniPos, endPos;
				Quat iniRot, endRot;
				getAnimationsData(instances[id - 1], anim->bones[i], iniPos, endPos, iniRot, endRot);*/

				float pos = (float)(instances[id - 1]->localTime * anim->joints[i]->positions.size() - 1) / (float)anim->duration;
				float rot = (float)(instances[id - 1]->localTime * anim->joints[i]->rotations.size() - 1) / (float)anim->duration;

				unsigned int posEndIndex, rotEndIndex;
				(pos >= anim->joints[i]->positions.size() - 1) ? posEndIndex = 0 : posEndIndex = (unsigned int)(pos + 1);
				(rot >= anim->joints[i]->rotations.size() - 1) ? rotEndIndex = 0 : rotEndIndex = (unsigned int)(rot + 1);

				float3 iniPos = anim->joints[i]->positions[(unsigned int)pos];
				float3 endPos = anim->joints[i]->positions[posEndIndex];
				Quat iniRot = anim->joints[i]->rotations[(unsigned int)rot];
				Quat endRot = anim->joints[i]->rotations[rotEndIndex];

				position = linearInterpolationPosition(iniPos, endPos, pos-floor(pos));
				rotation = linearInterpolationRotation(iniRot, endRot, rot-floor(rot));

				//If it's blending, add the second animation
				if (instances[id - 1]->blendingAnim != nullptr)
				{
					float3 positionBlend;
					Quat rotationBlend;
					if (getTransformBlend(instances[id - 1]->blendingAnim, boneName, positionBlend, rotationBlend))
					{
						float time = (float)instances[id - 1]->blendTime / (float)instances[id - 1]->blendDuration;

						position = linearInterpolationPosition(position, positionBlend, time);
						rotation = linearInterpolationRotation(rotation, rotationBlend, time);
					}
				}

				success = true;
			}
		}
	}
	return success;
}

bool ModuleAnimation::getTransformBlend(const AnimationInstance* anim, const char* boneName, float3& position, Quat& rotation)
{
	bool success = false;
	Animation* blendAnim = anim->animation;
	for (unsigned int j = 0; j < blendAnim->joints.size() && !success; ++j)
	{
		if (blendAnim->joints[j]->name == boneName)
		{
			float pos = (float)(anim->localTime * blendAnim->joints[j]->positions.size() - 1) / (float)blendAnim->duration;
			float rot = (float)(anim->localTime * blendAnim->joints[j]->rotations.size() - 1) / (float)blendAnim->duration;

			unsigned int posEndIndex, rotEndIndex;
			(pos >= blendAnim->joints[j]->positions.size() - 1) ? posEndIndex = 0 : posEndIndex = (unsigned int)(pos + 1);
			(rot >= blendAnim->joints[j]->rotations.size() - 1) ? rotEndIndex = 0 : rotEndIndex = (unsigned int)(rot + 1);

			float3 iniPos = blendAnim->joints[j]->positions[(unsigned int)pos];
			float3 endPos = blendAnim->joints[j]->positions[posEndIndex];
			Quat iniRot = blendAnim->joints[j]->rotations[(unsigned int)rot];
			Quat endRot = blendAnim->joints[j]->rotations[rotEndIndex];

			position = linearInterpolationPosition(iniPos, endPos, pos - floor(pos));
			rotation = linearInterpolationRotation(iniRot, endRot, rot - floor(rot));

			success = true;
		}
	}
	return success;
}

void ModuleAnimation::blendTo(unsigned int id, const char* name, unsigned int blendTime)
{
	if (id != 0 && id <= instances.size() && instances[id - 1] != nullptr)
	{
		AnimationInstance* anim = instances[id - 1];
		std::map<std::string, Animation*>::iterator it = animations.find(name);
		if (it != animations.end())
		{
			AnimationInstance* blendAnim = new AnimationInstance();
			blendAnim->animation = (*it).second;
			anim->blendingAnim = blendAnim;
			anim->blendDuration = blendTime;
			if (anim->blendDuration < 0)
			{
				anim->blendDuration = 0;
			}
		}
	}
}

float3 ModuleAnimation::linearInterpolationPosition(const float3& iniPos, const float3& endPos, float time) const
{
	return iniPos*(1.f-time)+endPos*time;
}

Quat ModuleAnimation::linearInterpolationRotation(const Quat& iniRot, const Quat& endRot, float time) const
{
	Quat rotation = { 0.f, 0.f, 0.f, 0.f };
	float dot = iniRot.Dot(endRot);
	float scalar = time;

	if (dot < 0.f)
	{
		scalar = -scalar;
	}

	rotation.x = iniRot.x*(1.f - time) + endRot.x*scalar;
	rotation.y = iniRot.y*(1.f - time) + endRot.y*scalar;
	rotation.z = iniRot.z*(1.f - time) + endRot.z*scalar;
	rotation.w = iniRot.w*(1.f - time) + endRot.w*scalar;

	return rotation;
}

bool ModuleAnimation::isBlending(unsigned int id)
{
	return instances[id - 1]->blendingAnim != nullptr;
}