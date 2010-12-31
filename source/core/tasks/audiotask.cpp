#include "audiotask.h"

using namespace OSS;


void AudioTask::addSoundEffect(SoundEffect * effect)
{
	if (!effect) return;
	
	//Add the sound effect to the appropriate layer
	soundEffects[effect->layer].push_back(effect);
	
	//Start playing the effect
	effect->play();
}

void AudioTask::playSound(Sound * sound, SoundEffect::Layer layer)
{
	addSoundEffect(new SoundEffect(sound, layer));
}

void AudioTask::update()
{
	//Search for stopped sound effects and remove them from the map
	for (SoundEffectListMap::iterator mapEntry = soundEffects.begin();
		 mapEntry != soundEffects.end();
		 mapEntry++) {
		for (SoundEffectList::iterator vectorEntry = mapEntry->second.begin();
			 vectorEntry != mapEntry->second.end();
			 vectorEntry++) {
			if ((*vectorEntry)->isStopped()) {
				OSSObjectLog << "removing SoundEffect with " << (*vectorEntry)->sound->name << std::endl;
				mapEntry->second.erase(vectorEntry);
			}
		}
	}
}
