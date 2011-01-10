#ifndef OSS_ENGINE_AUDIO_AUDIO_H
#define OSS_ENGINE_AUDIO_AUDIO_H

#include "../external.h"

#include "soundeffect.h"

//OpenAL
#ifdef __APPLE__
#include <OpenAL/al.h>
//#include <OpenAL/alc.h>
#include "../../os/macosx/alc.h" //required since alc.h in OS X uses typedef'd void for empty param list -> C++ sucks!
#endif


namespace OSS {
	namespace Engine {
		class Audio : public Core::Responder, public Core::Currentable<Audio> {
			
			/**
			 * Construction
			 */
		public:
			const Base::Pointer<Core::Application> application;
			
			Audio(Core::Application * application);
			~Audio();
			
			
			/**
			 * Sound Effects
			 */
		private:
			typedef std::list< Pointer<SoundEffect> > SoundEffectList;
			typedef std::map< SoundEffect::Layer, SoundEffectList > SoundEffectListMap;
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
}


#endif
