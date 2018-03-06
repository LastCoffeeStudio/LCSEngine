#include "ListenerComponent.h"
#include "ModuleAudio.h"
#include "GameObject.h"
#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include "Application.h"
#include "Globals.h"

ListenerComponent::ListenerComponent(GameObject* gameObject) : Component(gameObject, true, true)
{
    idAudioGameObj = App->audio->registerGameObj(gameObject->name.c_str());
    App->audio->setListener(idAudioGameObj);
    typeComponent = LISTENER;
}


ListenerComponent::~ListenerComponent()
{
}

void ListenerComponent::drawGUI()
{
}
//Update