#ifndef __AUDIOSOURCECOMPONENT_H__
#define __AUDIOSOURCECOMPONENT_H__

#include "Component.h"

class AudioSourceComponent : public Component
{
public:
    AudioSourceComponent(GameObject* gameObject);
    ~AudioSourceComponent();
    void drawGUI() override;
	void load(nlohmann::json& conf) override;
	void save(nlohmann::json& conf) override;

public:

    int idAudioGameObj;
private:

public:

};

#endif //__AUDIOSOURCECOMPONENT_H__