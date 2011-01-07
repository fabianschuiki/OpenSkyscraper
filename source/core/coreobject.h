#ifndef OSS_COREOBJECT_H
#define OSS_COREOBJECT_H

#include "../general.h"
#include "../base/object.h"
#include "coreevent.h"


namespace OSS {
	class CoreObject : public Object {
	public:
		//Events
		virtual bool handleEvent(CoreEvent * event);
		
		/**
		 * Event Senders
		 */
		void sendPrepare();
		void sendCleanUp();
		
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
