#ifndef OSS_EVENT_H
#define OSS_EVENT_H

#include "../general.h"


namespace OSS {
	/**
	 * Base events. This list may be extended by other parts of the engine
	 * by using their own enum starting from kNumEventsBase.
	 */
	enum {
		kEventNone = 0,
		kEventSDL,
		
		kNumEventsBase
	};
	
	/**
	 * SDL event wrapper that allows SDL events to be sent down the
	 * engine's own event pipe.
	 */
	typedef struct {
		unsigned int type;
		SDL_Event * event;
	} EventSDL;
	
	/**
	 * Basic event union. Other parts of the engine may extend this by
	 * creating a new union and defining "Event base" as one of its
	 * unions.
	 */
	typedef union {
		unsigned int type;
		EventSDL sdl;
	} Event;
}


#endif
