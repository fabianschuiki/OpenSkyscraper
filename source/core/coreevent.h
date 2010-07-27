#ifndef COREEVENT_H
#define COREEVENT_H

#include "../general.h"
#include "../base/event.h"


namespace OSS {
	/**
	 * Core events
	 */
	typedef enum {
		//Birth and death
		kCoreEventPrepare = kNumEventsBase,
		kCoreEventCleanUp,
		
		//Video Mode
		kCoreEventVideoModeChanged,
		
		//Scene
		kCoreEventSceneMovedOnScreen,
		kCoreEventSceneMovedOffScreen,
		
		kNumEventsCore
	};
	
	/**
	 * Scene events. Called by the engine whenever a scene moves on or off the screen.
	 */
	class Scene; //we don't want to have headers that are nested too much
	typedef struct {
		unsigned int type;
		Scene * scene;
	} CoreEventScene;
	
	/**
	 * Core event union
	 */
	typedef union {
		unsigned int type;
		Event base;
		CoreEventScene scene;
	} CoreEvent;
}


#endif
