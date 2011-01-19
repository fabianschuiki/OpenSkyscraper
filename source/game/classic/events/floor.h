#ifndef OSS_CLASSIC_EVENTS_FLOOR_H
#define OSS_CLASSIC_EVENTS_FLOOR_H

#include "event.h"


namespace OSS {
	namespace Classic {
		class FloorEvent : public Event {
		public:
			const int floor;
			
			FloorEvent(Event::Type type, int floor)
			: Event(type), floor(floor) {}
		};
	}
}


#endif
