#ifndef OSS_ENGINE_EVENTS_SDL_H
#define OSS_ENGINE_EVENTS_SDL_H

#include "../event.h"


namespace OSS {
	class SDLEvent : public Event {
	public:
		const SDL_Event event;	//the original SDL event structure
		
		SDLEvent(SDL_Event event) : Event(Event::kSDL), event(event) {}
	};
}


#endif
