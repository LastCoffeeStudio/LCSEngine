#ifndef __AUDIOLISTENERCOMPONENT_H__
#define __AUDIOLISTENERCOMPONENT_H__

#include "Component.h"

class AudioListenerComponent : public Component
{
public:
    AudioListenerComponent(GameObject* gameObject);
    ~AudioListenerComponent();
    void drawGUI() override;
	void load(nlohmann::json& conf) override;
	void save(nlohmann::json& conf) override;
    
public:
    
    int idAudioGameObj;
private:

public:
    
};

#endif //__AUDIOLISTENERCOMPONENT_H__