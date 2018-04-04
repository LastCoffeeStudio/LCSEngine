#ifndef __LIGHTCOMPONENT_H__
#define __LIGHTCOMPONENT_H__

#include "Component.h"

class LightComponent : public Component
{
public:
	LightComponent(GameObject* gameObject);
	~LightComponent();
	void drawGUI() override;
	void load(nlohmann::json& conf) override;
	void save(nlohmann::json& conf) override;
};

#endif //__LIGHTCOMPONENT_H__

