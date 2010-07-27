#ifndef CORE_OBJECT_H
#define CORE_OBJECT_H

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
		virtual bool eventPrepare()	{ return false; }
		virtual bool eventCleanUp()	{ return false; }
		
		//Video Mode
		virtual bool eventVideoModeChanged()	{ return false; }
	};
}


#endif
