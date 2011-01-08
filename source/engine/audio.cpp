#include "audio.h"

using namespace OSS;
using namespace Engine;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

Audio::Audio(Core::Application * application) : Core::Responder(), application(application)
{
	//DEBUG: Setup some OpenAL context so we can at least play sounds
	ALCdevice * device = alcOpenDevice(NULL);
	OSSObjectLog << "initializing audio device " << alcGetString(device, ALC_DEVICE_SPECIFIER) << std::endl;
	ALCcontext * context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);
	alListenerfv(AL_POSITION, (ALfloat[]){0, 0, 0});
	alListenerfv(AL_VELOCITY, (ALfloat[]){0, 0, 0});
	alListenerfv(AL_ORIENTATION, (ALfloat[]){0, 0, 1,  0, 1, 0});
}

Audio::~Audio()
{
	//TODO: Release OpenAL properly
}
