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
		
		/**
		 * Tells the AudioTask to duplicate the SoundEffect before playback. Set this to true if
		 * you intend to create a template SoundEffect which is kept around in your class and sent
		 * to the AudioTask multiple times.
		 */
		bool copyBeforeUse;
		
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
