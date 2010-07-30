#ifndef OSS_CORE_OBJECT_H
#define OSS_CORE_OBJECT_H

#include "../general.h"
#include "../base/object.h"
#include "coreevent.h"


namespace OSS {
	class CoreObject : public Object {
	public:
		//Events
		virtual bool handleEvent(CoreEvent * event);
		
		/**
		 * Event Handlers
		 */
		
		//Birth and death
		virtual void eventPrepare()	{}
		virtual void eventCleanUp()	{}
		
		//Video Mode
		virtual void eventVideoModeChanged()	{}
		
		//Scene
		virtual void eventSceneMovedOnScreen(Scene * scene)		{}
		virtual void eventSceneMovedOffScreen(Scene * scene)	{}
	};
}


#endif
