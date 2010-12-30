#include "sound.h"
#include "../classes.h"

using namespace OSS;


//Store
SoundStore * Sound::store;


//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

Sound::Sound(std::string name) : StoreItem(name)
{
	bufferID = 0;
}

Sound::~Sound()
{
	//Delete the OpenAL buffer
	if (bufferID) {
		alDeleteBuffers(1, &bufferID);
		bufferID = 0;
	}
}

std::string Sound::instanceName()
{
	return this->className() + " " + name;
}

void Sound::assignLoadedData(const void * data, ALuint length)
{
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Loading / Unloading
//----------------------------------------------------------------------------------------------------

void Sound::load()
{
}

void Sound::finalize()
{
}


void Sound::unfinalize()
{
}

void Sound::unload()
{
}
