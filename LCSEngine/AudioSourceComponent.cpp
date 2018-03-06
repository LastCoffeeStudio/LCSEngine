#include "AudioSourceComponent.h"
#include "ModuleAudio.h"
#include "GameObject.h"
#include "Application.h"
#include "Globals.h"
#include "Imgui/imgui.h"
#include "Wwise/WwiseProject/LCSEngineWwise/GeneratedSoundBanks/Wwise_IDs.h"


AudioSourceComponent::AudioSourceComponent(GameObject* gameObject) : Component(gameObject, true, true)
{
    idAudioGameObj = App->audio->registerGameObj(gameObject->name.c_str());
    typeComponent = AUDIOSOURCE;
}


AudioSourceComponent::~AudioSourceComponent() {}

void AudioSourceComponent::drawGUI()
{
    if (ImGui::CollapsingHeader("Audio Source"))
    {
        const char* items[] = {  "SHOTGUN", "FARMACIA" };
        static int item2 = 0;
        if(ImGui::Combo("Sound", &item2, items, IM_ARRAYSIZE(items)))
        {
            if(item2 == 0)
            {
               App->audio->eventAudio = AK::EVENTS::PLAY_GUNSHOT;
            }else
            {
                App->audio->eventAudio = AK::EVENTS::PLAY_FARMAC;
            }
        }
    }
}
