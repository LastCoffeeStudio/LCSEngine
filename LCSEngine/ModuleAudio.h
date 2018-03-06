#ifndef __MODULEAUDIO_H__
#define __MODULEAUDIO_H__

#include <vector>
#include "Module.h"
#include <AK/Tools/Common/AkObject.h>

#define DEFAULT_MUSIC_FADE_TIME 2.0f

namespace math {
    class float4x4;
}

struct _Mix_Music;
struct Mix_Chunk;
class TransformComponent;
typedef struct _Mix_Music Mix_Music;

class ModuleAudio : public Module
{
public:

	ModuleAudio(bool start_enabled = true);
	~ModuleAudio();

	bool init();
	update_status update(float deltaTime);
	bool cleanUp();
    AkGameObjectID registerGameObj(const char* name);
    void setListener(AkGameObjectID listenerId);
    void updatePositionListener(AkGameObjectID objectId, const math::float4x4& transform);
    void updatePositionAudioSource(AkGameObjectID objectId, const math::float4x4& transform);
private:
    AkGameObjectID nextIdGameObjSound = 0;
    AkGameObjectID currentAudioListener = -1;
};

#endif // __MODULEAUDIO_H__