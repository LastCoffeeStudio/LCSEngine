#include "AudioSourceComponent.h"
#include "ModuleAudio.h"
#include "GameObject.h"
#include "Application.h"
#include "Globals.h"


AudioSourceComponent::AudioSourceComponent(GameObject* gameObject) : Component(gameObject, true, true)
{
    idAudioGameObj = App->audio->registerGameObj(gameObject->name.c_str());
    typeComponent = AUDIOSOURCE;
}


AudioSourceComponent::~AudioSourceComponent()
{
}

void AudioSourceComponent::drawGUI()
{
    
}
