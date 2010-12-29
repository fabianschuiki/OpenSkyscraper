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
}

Sound::~Sound()
{
}

std::string Sound::instanceName()
{
	return this->className() + " " + name;
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
