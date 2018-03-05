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
	anim->duration = scene->mAnimations[0]->mDuration;
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

	animations[scene->mAnimations[0]->mName.C_Str()] = anim;

	return true;
}

update_status ModuleAnimation::update(float deltaTime)
{
	return UPDATE_CONTINUE;
}

unsigned int ModuleAnimation::play(const char* name)
{
	unsigned int id = -1;
	return id;
}

void ModuleAnimation::stop(unsigned int id) {}

bool ModuleAnimation::getTransform(unsigned int id, const char* frameName, float3& position, Quat& rotation)
{
	bool success = false;
	return success;
}