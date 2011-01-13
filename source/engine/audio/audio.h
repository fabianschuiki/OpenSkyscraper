#ifndef OSS_ENGINE_AUDIO_AUDIO_H
#define OSS_ENGINE_AUDIO_AUDIO_H

#include "../external.h"
#include "../base/object.h"
#include "../base/singleton.h"

#include "../engine.h"
#include "soundeffect.h"


namespace OSS {	
	class Audio : public Object, public Singleton<Audio> {
		
		/**
		 * Initialization
		 */
	public:
		const Pointer<Engine> engine;
		
		Audio(Engine * engine);
		~Audio();
		
		
		/**
		 * Sound Effects
		 */
	private:
		typedef list< Pointer<SoundEffect> > SoundEffectList;
		typedef map< SoundEffect::Layer, SoundEffectList > SoundEffectListMap;
		SoundEffectListMap soundEffects;
		
	public:
		void play(SoundEffect * effect);
		void play(Sound * sound, SoundEffect::Layer layer);
		
		
		/**
		 * State
		 */
	private:
		bool disabled;
		
	public:
		void enableSoundEffects();
		void disableSoundEffects();
		
		virtual void update();
	};
}


#endif
