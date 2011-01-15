#ifndef OSS_CLASSIC_EVENT_H
#define OSS_CLASSIC_EVENT_H

#include "../../external.h"


namespace OSS {
	namespace Classic {
		class Event : public OSS::Event {
		public:
			typedef enum {
				kNoEvent = 0,
				
				//Time
				kTimeChanged,
				kDayChanged,
				kQuarterChanged,
				kYearChanged,
				
				//UI
				kToolChanged,
				
				//Transport
				kTransportIncreased,
				kTransportDecreased,
				
				//Hotel
				kHotelVacated
			} Type;
			
		private:
			const Type classicType;
			
		public:
			Event() : OSS::Event(), classicType(kNoEvent) {}
			Event(Type t) : OSS::Event(), classicType(t) {}
			
			bool isType(Type t) {
				return (classicType == t);
			}
			virtual string getCategory() { return "classic"; }
		};
	}
}

//Include the various events
#include "item.h"


#endif
