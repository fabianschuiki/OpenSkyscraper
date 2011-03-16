#include "soundeffect.h"

using namespace OSS;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Initialization
//----------------------------------------------------------------------------------------------------

SoundEffect::SoundEffect()
{
	init();
}

SoundEffect::SoundEffect(const SoundEffect & e)
{
	OSSObjectLog << "copying sound effect" << std::endl;
	*this = e;
	retainCount = 1;
}

SoundEffect::SoundEffect(Sound * sound, Layer layer)
{
	init();
	this->sound = sound;
	this->layer = layer;
}

void SoundEffect::init()
{
	layer = kTopLayer;
	sourceID = 0;
	copyBeforeUse = false;
	maxConcurrentPlaybacks = 0;
	minIntervalBetweenPlaybacks = 0;
	loopCount = 1;
	loopInfinitely = false;
	volume = 1.0;
	fade = 1.0;
}

SoundEffect::~SoundEffect()
{
	OSSObjectLog << std::endl;
	
	//Stop playback, this will also get rid of the source if necessary
	stop();
}

string SoundEffect::instanceName()
{
	return this->className() + " " + (sound ? sound->name : "<no sound>");
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Playback
//----------------------------------------------------------------------------------------------------

void SoundEffect::play()
{
	//If there's no source yet...
	if (!sourceID) {
		//Create the source
		alGenSources(1, &sourceID);
		
		//Setup some default source attributes
		alSourcei(sourceID, AL_BUFFER, sound->bufferID);
		alSourcei(sourceID, AL_SOURCE_RELATIVE, AL_TRUE);
		
		//Decide whether to loop
		cachedSecondsPlayed = 0;
		loopsLeft = (loopCount - 1);
		if (loopsLeft || loopInfinitely)
			alSourcei(sourceID, AL_LOOPING, AL_TRUE);
	}
	
	//Start playing the source
	alSourcePlay(sourceID);
}

void SoundEffect::pause()
{
	if (!sourceID) return;
	
	//Pause the playback
	alSourcePause(sourceID);
}

void SoundEffect::stop()
{
	if (!sourceID) return;
	
	//Stop playing
	alSourceStop(sourceID);
	
	//Get rid of the source
	alDeleteSources(1, &sourceID);
	sourceID = 0;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

ALint SoundEffect::getSourceState()
{
	ALint state;
	alGetSourcei(sourceID, AL_SOURCE_STATE, &state);
	return state;
}

bool SoundEffect::isPlaying()
{
	return (getSourceState() == AL_PLAYING);
}

bool SoundEffect::isStopped()
{
	return (getSourceState() == AL_STOPPED);
}

double SoundEffect::getSecondsPlayed()
{
	if (!sourceID || !sound) return 0;
	
	//WTF? alGetSourcef returns a float that without any fractional part! So we have to do the
	//magic ourselves.
	ALint samples;
	alGetSourcei(sourceID, AL_SAMPLE_OFFSET, &samples);
	ALint frequency;
	alGetBufferi(sound->bufferID, AL_FREQUENCY, &frequency);
	return ((double)samples / frequency);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Heartbeat
//----------------------------------------------------------------------------------------------------

void SoundEffect::update()
{
	if (isPlaying()) {
		//Get the seconds played
		double secondsPlayed = getSecondsPlayed();
		
		//If the seconds played is lower than the previous one, we looped
		if (secondsPlayed < cachedSecondsPlayed && !loopInfinitely) {
			if (--loopsLeft == 0)
				alSourcei(sourceID, AL_LOOPING, AL_FALSE);
		}
		
		//Set the volume
		alSourcef(sourceID, AL_GAIN, volume * fade);
		
		//Cache the seconds played for the next heartbeat
		cachedSecondsPlayed = secondsPlayed;
	}
}
