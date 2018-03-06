#ifndef __AUDIOSOURCECOMPONENT_H__
#define __AUDIOSOURCECOMPONENT_H__

#include "Component.h"

class AudioSourceComponent : public Component
{
public:
    AudioSourceComponent(GameObject* gameObject);
    ~AudioSourceComponent();
    void drawGUI() override;

public:

    int idAudioGameObj;
private:

public:

};

#endif //__AUDIOSOURCECOMPONENT_H__