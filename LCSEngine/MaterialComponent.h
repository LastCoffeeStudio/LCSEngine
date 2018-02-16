#ifndef __MATERIALCOMPONENT_H__
#define __MATERIALCOMPONENT_H__

#include "Component.h"
#include "Glew/include/glew.h"
#include <string>

class AssetTexture;

class MaterialComponent : public Component
{
public:
	MaterialComponent(GameObject* gameObject, bool isEnable = true);
	~MaterialComponent();
	void drawGUI();

public:
	GLuint program = 0;

private:
	AssetTexture * texture = nullptr;
	std::string shaderName = "Default Shader";
	std::string textureName = "None";
	float texTiling[2] = { 1.f, 1.f };
	float texOffset[2] = { 0.f, 0.f };
};

#endif //__MATERIALCOMPONENT_H__