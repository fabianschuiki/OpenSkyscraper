#ifndef OSS_CORE_EVENTS_MOUSE_H
#define OSS_CORE_EVENTS_MOUSE_H

#include "../event.h"

#include "external.h"


namespace OSS {
	namespace Core {
		class MouseEvent : public Event {
		public:
			const int2 position;	//mouse position when the event happened
			
			MouseEvent(Event::Type type, int2 position)
			: Event(type), position(position) {}
		};
	}
}


#endif
