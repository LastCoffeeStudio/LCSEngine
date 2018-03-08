#ifndef __AUDIOREVERBCOMPONENT_H__
#define __AUDIOREVERBCOMPONENT_H__

#include "Component.h"

class AudioReverbComponent : public Component
{
public:
	AudioReverbComponent(GameObject* gameObject);
	~AudioReverbComponent();
	void drawGUI() override;

};

#endif //__AUDIOREVERBCOMPONENT_H__
