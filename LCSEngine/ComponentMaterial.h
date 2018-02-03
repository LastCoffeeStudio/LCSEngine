#ifndef __COMPONENTMATERIAL_H__
#define __COMPONENTMATERIAL_H__

#include "Component.h"
#include <string>

using namespace std;

class Shader;
class AssetTexture;

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* gameObject, bool isEnable = true);
	~ComponentMaterial();
	void drawGUI();

private:
	AssetTexture * texture = nullptr;
	string shaderName = "Default Shader";
	string textureName = "None";
	float texTiling[2] = {1.f, 1.f};
	float texOffset[2] = {0.f, 0.f};
};

#endif //__COMPONENTMATERIAL_H__