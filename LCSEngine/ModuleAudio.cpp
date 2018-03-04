#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "SDL/include/SDL.h"

#include "Wwise/include/SoundEngine.h"

using namespace std;

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
	
	return true;
}

// Called before quitting
bool ModuleAudio::cleanUp()
{
	
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