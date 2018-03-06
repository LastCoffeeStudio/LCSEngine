#ifndef __LISTENERCOMPONENT_H__
#define __LISTENERCOMPONENT_H__

#include "Component.h"

class ListenerComponent : public Component
{
public:
    ListenerComponent(GameObject* gameObject);
    ~ListenerComponent();
    void drawGUI() override;
    
public:
    
    int idAudioGameObj;
private:

public:
    
};

#endif //__LISTENERCOMPONENT_H__