#ifndef OSS_CORE_EVENT_H
#define OSS_CORE_EVENT_H

#include "../base/event.h"


namespace OSS {
	namespace Core {
		class Event : public Base::Event {
		public:
			synthesizeClassInheritance(Base::Event);
			
			/**
			 * Types
			 */
		public:
			typedef enum {
				None = 0,
				
				MouseButton,
				MouseDragged,
				MouseMoved,
				ScrollWheel,
				Key
			} Type;
			
			
			/**
			 * Core Event
			 */
		private:
			const Type coreType;
			
		public:
			Event() : Base::Event(), coreType(None) {}
			Event(Type t) : Base::Event(), coreType(t) {}
			
			bool isType(Type t) {
				return (coreType == t);
			}
		};
	}
}


//Include the various events
#include "events/mousebutton.h"
#include "events/mousedragged.h"
#include "events/mousemoved.h"
#include "events/scrollwheel.h"
#include "events/key.h"


#endif
