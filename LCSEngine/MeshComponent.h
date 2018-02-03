#ifndef __MESHCOMPONENT_H__
#define __MESHCOMPONENT_H__

#include "Component.h"

class MeshComponent : public Component
{
public:
	MeshComponent(GameObject* gameObject, std::string name, bool isEnable = false, bool isUnique = false);
	~MeshComponent();

	void drawGUI() override;

public:
	TypeComponent type;
};

#endif