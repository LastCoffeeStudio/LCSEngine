#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "SDL/include/SDL.h"
#include "ModuleInput.h"

#include "Wwise/include/SoundEngine.h"
#include "Wwise/WwiseProject/LCSEngineWwise/GeneratedSoundBanks/Wwise_IDs.h"

using namespace std;

#define BANKNAME_INIT L"Init.bnk"
#define BANKNAME_SHOTGUN L"Init_Bank.bnk"

AkGameObjectID MY_DEFAULT_LISTENER = 0;
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
		LOG("Cannot load the shotgun bank");
	}

	// Register the main listener.
	AK::SoundEngine::RegisterGameObj(MY_DEFAULT_LISTENER, "My Default Listener");

	// Set one listener as the default.
	AK::SoundEngine::SetDefaultListeners(&MY_DEFAULT_LISTENER, 1);

	AK::SoundEngine::RegisterGameObj(GAME_OBJ, "Gun");

	return true;
}

update_status ModuleAudio::update(float deltaTime)
{
	AK::SOUNDENGINE_DLL::Tick();

	if (App->input->getKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		AkPlayingID id = AK::SoundEngine::PostEvent(
			AK::EVENTS::PLAY_GUNSHOT,      // Unique ID of the event
			GAME_OBJ               // Associated game object ID
		);

		if (id == AK_INVALID_PLAYING_ID)
		{
			LOG("Me cago en mi puta madre (con cariño)");
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

// Play a music file
bool ModuleAudio::playMusic(const char* path, float fade_time)
{
	bool ret = true;


	return ret;
}

// Load WAV
unsigned int ModuleAudio::loadFx(const char* path)
{
	unsigned int ret = 0;
	

	return ret;
}

// Play WAV
bool ModuleAudio::playFx(unsigned int id, int repeat)
{
	bool ret = true;


	return ret;
}