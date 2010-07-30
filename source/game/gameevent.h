#ifndef OSS_GAMEEVENT_H
#define OSS_GAMEEVENT_H

#include "../general.h"
#include "../core/coreevent.h"


namespace OSS {
	/**
	 * Game Events
	 */
	enum {
	};
	
	/**
	 * Game event union
	 */
	typedef union {
		unsigned int type;
		CoreEvent core;
	} GameEvent;
}


#endif
