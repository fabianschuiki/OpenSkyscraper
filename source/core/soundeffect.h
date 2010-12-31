#ifndef OSS_SOUNDEFFECT_H
#define OSS_SOUNDEFFECT_H

#include "../general.h"
#include "coreobject.h"
#include "../resources/sound.h"


namespace OSS {
	class SoundEffect : public CoreObject {
	public:
		typedef enum {
			kBackgroundLayer,
			kTransportLayer,
			kTopLayer
		} Layer;
		
		Pointer<Sound> sound;
		Layer layer;
		ALuint sourceID;
		
		//Constructor
		SoundEffect();
		SoundEffect(Sound * sound, Layer layer);
		~SoundEffect();
		
		//Playback
		void play();
		void pause();
		void stop();
		
		//State
		ALint getSourceState();
		bool isPlaying();
		bool isStopped();
	};
}


#endif
