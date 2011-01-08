#ifndef OSS_AUDIOTASK_H
#define OSS_AUDIOTASK_H

#include "../../general.h"
#include "task.h"
#include "../soundeffect.h"


namespace OSS {
	class AudioTask : public Task {		
		//Sound Effects
		typedef std::list< Pointer<SoundEffect> > SoundEffectList;
		typedef std::map< SoundEffect::Layer, SoundEffectList > SoundEffectListMap;
		SoundEffectListMap soundEffects;
		
	public:
		//Initialization
		AudioTask();
		
		//Playing sounds
		void addSoundEffect(SoundEffect * effect);
		
		//Convenience
		void playSound(Sound * sound, SoundEffect::Layer layer);
		
	private:
		bool disabled;
	public:
		void enableSoundEffects();
		void disableSoundEffects();
		
	protected:
		friend class Engine;
		void update();
	};
}


#endif
