#include "AudioReverbComponent.h"


AudioReverbComponent::AudioReverbComponent(GameObject* gameObject) : Component(gameObject, true, true)
{
	typeComponent = AUDIOREVERB;
}

AudioReverbComponent::~AudioReverbComponent(){}

void AudioReverbComponent::drawGUI()
{
	
}
