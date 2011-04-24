#include "audio.h"

using namespace OSS;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

Audio::Audio(Engine * engine) : engine(engine)
{
	//DEBUG: Setup some OpenAL context so we can at least play sounds
	ALCdevice * device = alcOpenDevice(NULL);
	OSSObjectLog << "initializing audio device " << alcGetString(device, ALC_DEVICE_SPECIFIER) << std::endl;
	ALCcontext * context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);
	alListenerfv(AL_POSITION, (ALfloat[]){0, 0, 0});
	alListenerfv(AL_VELOCITY, (ALfloat[]){0, 0, 0});
	alListenerfv(AL_ORIENTATION, (ALfloat[]){0, 0, 1,  0, 1, 0});
	
	disabled = false;
}

Audio::~Audio()
{
	//TODO: Release OpenAL properly
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Sound Effects
//----------------------------------------------------------------------------------------------------

void Audio::play(SoundEffect * effect)
{
	if (!effect || disabled) return;
	
	//Apply the concurrency limitation
	if (effect->maxConcurrentPlaybacks || effect->minIntervalBetweenPlaybacks) {
		//Count the number of times the same sound is being played and the shortest duration for how
		//long it's been playing.
		unsigned int concurrentPlaybacks = 0;
		double shortest = 1e10; //some big number
		SoundEffectList * effects = &soundEffects[effect->layer];
		for (SoundEffectList::iterator e = effects->begin(); e != effects->end(); e++) {
			if ((*e)->sound == effect->sound) {
				concurrentPlaybacks++;
				shortest = std::min<double>(shortest, (*e)->getSecondsPlayed());
			}
		}
		
		//Reject the sound effect if this exceeds the maximum
		if (effect->maxConcurrentPlaybacks && concurrentPlaybacks >= effect->maxConcurrentPlaybacks) {
			/*OSSObjectLog << "rejecting " << effect->description()
			<< " due to exceeded concurrent playbacks" << std::endl;*/
			return;
		}
		
		//Reject the sound effect if the minimal interval has not been reached yet
		if (shortest < effect->minIntervalBetweenPlaybacks) {
			/*OSSObjectLog << "rejecting " << effect->description()
			<< " due to limitation of minimal interval between playbacks ("
			<< shortest << " < " << effect->minIntervalBetweenPlaybacks << ")" << std::endl;*/
			return;
		}
	}
	
	//Copy the sound effect if required
	if (effect->copyBeforeUse)
		effect = new SoundEffect(*effect);
	
	//Add the sound effect to the appropriate layer
	soundEffects[effect->layer].push_back(effect);
	
	//Start playing the effect
	effect->play();
}

void Audio::play(Sound * sound, SoundEffect::Layer layer)
{
	play(new SoundEffect(sound, layer));
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

void Audio::enableSoundEffects()
{
	disabled = false;
}

void Audio::disableSoundEffects()
{
	disabled = true;
}

void Audio::update()
{
	//Iterate through all sound effects currently playing
	for (SoundEffectListMap::iterator mapEntry = soundEffects.begin();
		 mapEntry != soundEffects.end();
		 mapEntry++) {
		for (SoundEffectList::iterator listEntry = mapEntry->second.begin();
			 listEntry != mapEntry->second.end();
			 listEntry++) {
			
			//Update
			(*listEntry)->update();
			
			//Get rid of stopped effects
			if ((*listEntry)->isStopped()) {
				//OSSObjectLog << "removing SoundEffect with " << (*listEntry)->sound->name << std::endl;
				mapEntry->second.erase(listEntry);
				--listEntry;
			}
		}
	}
}
