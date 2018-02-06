#ifndef __MATERIALCOMPONENT_H__
#define __MATERIALCOMPONENT_H__

#include "Component.h"
#include <string>

using namespace std;

class Shader;
class AssetTexture;

class MaterialComponent : public Component
{
public:
	MaterialComponent(GameObject* gameObject, bool isEnable = true, bool isUnique = false);
	~MaterialComponent();
	void drawGUI();

private:
	AssetTexture * texture = nullptr;
	string shaderName = "Default Shader";
	string textureName = "None";
	float texTiling[2] = { 1.f, 1.f };
	float texOffset[2] = { 0.f, 0.f };
};

#endif //__MATERIALCOMPONENT_H__