#include "AudioListenerComponent.h"
#include "ModuleAudio.h"
#include "GameObject.h"
#include "Application.h"
#include "Globals.h"
#include "Imgui/imgui.h"

AudioListenerComponent::AudioListenerComponent(GameObject* gameObject) : Component(gameObject, true, true)
{
    idAudioGameObj = App->audio->registerGameObj(gameObject->name.c_str());
    App->audio->setListener(idAudioGameObj);
    typeComponent = AUDIOLISTENER;
}


AudioListenerComponent::~AudioListenerComponent() {}

void AudioListenerComponent::drawGUI()
{
    if (ImGui::CollapsingHeader("AudioListener"))
    {
       
    }
}

void AudioListenerComponent::load(nlohmann::json& conf)
{
	Component::load(conf);
	typeComponent = AUDIOLISTENER;
}

void AudioListenerComponent::save(nlohmann::json& conf)
{
	Component::save(conf);
	nlohmann::json customJsont;
}
