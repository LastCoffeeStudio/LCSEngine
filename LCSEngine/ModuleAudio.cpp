#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleCamera.h"
#include "CameraComponent.h"
#include "SDL/include/SDL.h"
#include "ModuleInput.h"

#include "Wwise/include/SoundEngine.h"
#include "Wwise/WwiseProject/LCSEngineWwise/GeneratedSoundBanks/Wwise_IDs.h" 
#include "TransformComponent.h"
using namespace std;

#define BANKNAME_INIT L"Init.bnk"
#define BANKNAME_SHOTGUN L"Init_Bank.bnk"


AkGameObjectID GAME_OBJ = 100;

ModuleAudio::ModuleAudio(bool start_enabled) : Module(start_enabled) {}

// Destructor
ModuleAudio::~ModuleAudio() {}

// Called before render is available
bool ModuleAudio::init()
{
	AkMemSettings memSettings;
	memSettings.uMaxNumPools = 20;

	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings(stmSettings);

	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);

	AK::SOUNDENGINE_DLL::Init(&memSettings, &stmSettings, &deviceSettings, &initSettings, &platformInitSettings, &musicInit);

	AK::SOUNDENGINE_DLL::SetBasePath(AKTEXT("../Wwise/WwiseProject/LCSEngineWwise/GeneratedSoundBanks/Windows/"));
	AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));
    
	AkBankID bankID; // Not used. These banks can be unloaded with their file name.
	AKRESULT eResult = AK::SoundEngine::LoadBank(BANKNAME_INIT, AK_DEFAULT_POOL_ID, bankID);
	if (eResult != AK_Success)
	{
		LOG("Cannot load the init bank");
	}
	eResult = AK::SoundEngine::LoadBank(BANKNAME_SHOTGUN, AK_DEFAULT_POOL_ID, bankID);
	if (eResult != AK_Success)
	{
		LOG("Cannot load the shotgun bank")
	}
    eventAudio = AK::EVENTS::PLAY_GUNSHOT;
    AK::SoundEngine::RegisterGameObj(GAME_OBJ, "Gun");
	reverb = false;

	return true;
}

update_status ModuleAudio::update(float deltaTime)
{
	AK::SOUNDENGINE_DLL::Tick();

	if (App->input->getKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		AkPlayingID id = AK::SoundEngine::PostEvent(
            eventAudio,      // Unique ID of the event
			GAME_OBJ               // Associated game object ID
		);

		if (id == AK_INVALID_PLAYING_ID)
		{
			LOG("Me cago en mi puta madre (con cariño)");
		}
	}

	if (App->input->getKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		reverb = !reverb;
		if (reverb)
		{
			AK::SoundEngine::SetGameObjectAuxSendValues(GAME_OBJ, nullptr, 0);
		}
		else
		{
			AkAuxSendValue aEnvs[1];
			aEnvs[0].listenerID = currentAudioListener; // Use the same set of listeners assigned via the SetListeners/SetDefaultListeners API.
			aEnvs[0].auxBusID = AK::AUX_BUSSES::REVERB_EFFECT;
			aEnvs[0].fControlValue = 1.0f;
			AK::SoundEngine::SetGameObjectAuxSendValues(GAME_OBJ, aEnvs, 2);
		}
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleAudio::cleanUp()
{
	AK::SoundEngine::UnloadBank(BANKNAME_SHOTGUN, NULL);
	AK::SoundEngine::UnregisterAllGameObj();

	AK::SOUNDENGINE_DLL::Term();

	return true;
}

AkGameObjectID ModuleAudio::registerGameObj(const char* name)
{
    AK::SoundEngine::RegisterGameObj(nextIdGameObjSound, name);
    ++nextIdGameObjSound;
    return (nextIdGameObjSound -1);
}

void ModuleAudio::setListener(AkGameObjectID listenerId)
{
    if(currentAudioListener < 0)
    {
       AK::SoundEngine::SetDefaultListeners(&listenerId, 1);
    }else
    {
       AK::SoundEngine::SetDefaultListeners(&listenerId, 1);
    }
	currentAudioListener = listenerId;
}

void ModuleAudio::unsetListener(AkGameObjectID listenerId)
{
	AK::SoundEngine::RemoveDefaultListener(listenerId);
}

void ModuleAudio::updatePositionListener(AkGameObjectID objectId, const float4x4& transform)
{
	AkListenerPosition listenerTransform;

    AkVector vecPosition;
    vecPosition.X = transform[3][0];
    vecPosition.Y = transform[3][1];
    vecPosition.Z = transform[3][2];
   
    AkVector vecUp;
    vecUp.X = transform[1][0];
    vecUp.Y = transform[1][1];
    vecUp.Z = transform[1][2];

    AkVector vecFront;
    vecFront.X = transform[2][0]*-1;
	vecFront.Y = transform[2][1]*-1;
	vecFront.Z = transform[2][2]*-1;

    listenerTransform.SetPosition(vecPosition);
    listenerTransform.SetOrientation(vecFront, vecUp);

    AK::SoundEngine::SetPosition(objectId, listenerTransform);
}

void ModuleAudio::updatePositionAudioSource(AkGameObjectID objectId, const math::float4x4& transform)
{
    AkSoundPosition soundPos;

    AkVector vecPosition;
    vecPosition.X = transform[3][0];
    vecPosition.Y = transform[3][1];
    vecPosition.Z = transform[3][2];

    AkVector vecUp;
    vecUp.X = transform[1][0];
    vecUp.Y = transform[1][1];
    vecUp.Z = transform[1][2];

    AkVector vecFront;
    vecFront.X = transform[2][0]*-1;
    vecFront.Y = transform[2][1]*-1;
    vecFront.Z = transform[2][2]*-1;

    soundPos.SetPosition(vecPosition);
    soundPos.SetOrientation(vecFront, vecUp);

    AK::SoundEngine::SetPosition(GAME_OBJ, soundPos);
}
