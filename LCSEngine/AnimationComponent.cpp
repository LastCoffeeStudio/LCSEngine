#include "Globals.h"
#include "Application.h"
#include "AnimationComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "ModuleAnimation.h"
#include "Glew/include/glew.h"
#include "Imgui/imgui.h"
#include <queue>

AnimationComponent::AnimationComponent(GameObject* gameObject) : Component(gameObject, true, true)
{
	typeComponent = ANIMATION;
}

AnimationComponent::~AnimationComponent() {}

void AnimationComponent::drawGUI()
{
	if (ImGui::CollapsingHeader("Animation"))
	{
		ImGui::Checkbox("Active", &isEnable);
		ImGui::Text("Animation: "); ImGui::SameLine(0);
		if (ImGui::BeginMenu(animationName.c_str()))
		{
			for (map<string, Animation*>::iterator it = App->animations->animations.begin(); it != App->animations->animations.end(); ++it)
			{
				if (ImGui::MenuItem((*it).first.c_str()))
				{
					animationName = (*it).first;
				}
			}

			ImGui::EndMenu();
		}
		if (ImGui::Button("Play"))
		{
			if (idAnim != 0)
			{
				blendAnimation();
			}
			else
			{
				playAnimation();
			}
		}
		ImGui::SameLine(0);
		if (ImGui::Button("Stop"))
		{
			stopAnimation();
		}
		ImGui::Text("Current Animation: %s", currentAnimationName.c_str());
		ImGui::DragFloat("Blend time", &blendTime, 0.05f, 0.f, 100.f);
	}
}

void AnimationComponent::drawHierarchy()
{
	if (rootBone != nullptr)
	{
		std::queue<GameObject*> children;

		for (vector<GameObject*>::iterator it = rootBone->children.begin(); it != rootBone->children.end(); ++it)
		{
			children.push(*it);
		}

		while (!children.empty())
		{
			GameObject* node = children.front();
			children.pop();

			for (vector<GameObject*>::iterator it = node->children.begin(); it != node->children.end(); ++it)
			{
				drawLine(node->id.Transposed(), (*it)->id.Transposed());
				children.push(*it);
			}
		}
	}
}

void AnimationComponent::drawLine(const float4x4& idParent, const float4x4& idChild)
{
	glBegin(GL_LINES);
	glVertex3f(idParent[0][3], idParent[1][3], idParent[2][3]);
	glVertex3f(idChild[0][3], idChild[1][3], idChild[2][3]);
	glEnd();
}

void AnimationComponent::playAnimation()
{
	idAnim = App->animations->play(animationName.c_str());
	if (idAnim > 0)
	{
		currentAnimationName = animationName;
	}
	else
	{
		LOG("Couldn't play the animation");
	}
}

void AnimationComponent::stopAnimation()
{
	if (idAnim > 0)
	{
		App->animations->stop(idAnim);
		idAnim = 0;
		currentAnimationName = "";
	}
}

void AnimationComponent::blendAnimation()
{
	if (!App->animations->isBlending(idAnim))
	{
		App->animations->blendTo(idAnim, animationName.c_str(), unsigned int(blendTime*1000));
	}
}