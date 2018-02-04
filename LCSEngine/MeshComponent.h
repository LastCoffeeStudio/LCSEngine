#ifndef __MESHCOMPONENT_H__
#define __MESHCOMPONENT_H__

#include "Component.h"

enum PresetType
{
	TRIANGLE = 1,
	CUBE,
	SPHERE
};

class MeshComponent : public Component
{
public:
	MeshComponent(GameObject* gameObject, bool isEnable = false, bool isUnique = false);
	~MeshComponent();

	void drawGUI() override;

public:
	PresetType currentPreset = TRIANGLE;
};

#endif