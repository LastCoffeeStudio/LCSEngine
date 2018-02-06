#ifndef __MESHCOMPONENT_H__
#define __MESHCOMPONENT_H__

#include "Component.h"
#include <vector>
#include <glew.h>

enum PresetType
{
	TRIANGLE = 1,
	CUBE,
	SPHERE
};

class MeshComponent : public Component
{
public:
	MeshComponent(GameObject* gameObject, bool isEnable = true, bool isUnique = true);
	~MeshComponent();

	void drawGUI() override;
	bool update() override;
	std::vector<float> verticesVBO;
	GLuint idVertVBO = 0;
	float lengthX, lengthY, lengthZ;

public:
	PresetType currentPreset = TRIANGLE;
};

#endif