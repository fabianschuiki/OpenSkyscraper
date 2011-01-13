#ifndef OSS_CLASSIC_EVENTS_ITEM_H
#define OSS_CLASSIC_EVENTS_ITEM_H

#include "event.h"


namespace OSS {
	namespace Classic {
		template <class T> class ItemEvent : public Event {
		public:
			const Pointer<T> item;
			
			ItemEvent(Event::Type type, T * item)
			: Event(type), item(item) {}
		};
	}
}


#endif
