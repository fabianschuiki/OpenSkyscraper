#ifndef OSS_ENGINE_EVENTS_AUDIO_H
#define OSS_ENGINE_EVENTS_AUDIO_H

#include "../../external.h"
#include "../event.h"


namespace OSS {
	class Audio;
	
	class AudioEvent : public Event {
	public:
		const Pointer<Audio> audio;
		
		AudioEvent(Event::Type type, Audio * audio)
		: Event(type), audio(audio) {}
	};
}


#include "../../audio/audio.h"


#endif
