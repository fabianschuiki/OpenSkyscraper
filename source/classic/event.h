#ifndef OSS_CLASSIC_EVENT_H
#define OSS_CLASSIC_EVENT_H

#include "external.h"


namespace OSS {
	namespace Classic {
		class Event : public Engine::Event {
		public:
			synthesizeClassInheritance(Engine::Event);
			
			
			/**
			 * Types
			 */
		public:
			typedef enum {
				None = 0,
				
				TransportIncreased,
				TransportDecreased,
				
				HotelVacated
			} Type;
			
			
			/**
			 * Classic Event
			 */
		private:
			const Type classicType;
			
		public:
			Event() : Engine::Event(), classicType(None) {}
			Event(Type t) : Engine::Event(), classicType(t) {}
			
			bool isType(Type t) {
				return (classicType == t);
			}
		};
	}
}

//Include the various events
#include "events/item.h"


#endif
