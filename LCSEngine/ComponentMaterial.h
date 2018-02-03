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
	void drawGui();

private:
	AssetTexture * texture = nullptr;
	string shaderName = "Default Shader";
};

#endif //__COMPONENTMATERIAL_H__