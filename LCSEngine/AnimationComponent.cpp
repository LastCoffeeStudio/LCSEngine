#include "AnimationComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
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
		ImGui::Checkbox("Active", &isEnable); ImGui::SameLine(0);
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