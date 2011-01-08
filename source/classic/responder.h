#ifndef OSS_CLASSIC_RESPONDER_H
#define OSS_CLASSIC_RESPONDER_H

#include "external.h"

#include "event.h"
#include "transportitem.h"
#include "items/hotelitem.h"


namespace OSS {
	namespace Classic {
		class Responder : public Engine::Responder {
			
			/**
			 * Event Handling
			 */
		public:
			virtual bool handleEvent(Base::Event * event);
			bool handleClassicEvent(Event * event);
			
			//Transport
			virtual void eventTransportIncreased(ItemEvent<TransportItem> * event) {}
			virtual void eventTransportDecreased(ItemEvent<TransportItem> * event) {}
			
			//Hotel
			virtual void eventHotelVacated(ItemEvent<HotelItem> * event) {}
		};
	}
}


#endif
