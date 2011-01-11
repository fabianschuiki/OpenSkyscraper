#ifndef OSS_ENGINE_EVENTS_AUDIO_H
#define OSS_ENGINE_EVENTS_AUDIO_H

#include "../external.h"
#include "../event.h"

#include "../audio/audio.h"


namespace OSS {
	namespace Engine {		
		class AudioEvent : public Event {
		public:
			const Base::Pointer<Audio> audio;
			
			AudioEvent(Event::Type type, Audio * audio)
			: Event(type), audio(audio) {}
		};
	}
}


#endif
