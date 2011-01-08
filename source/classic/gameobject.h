#ifndef OSS_GAMEOBJECT_H
#define OSS_GAMEOBJECT_H

#include "../general.h"
#include "../core/coreobject.h"
#include "gameevent.h"


namespace OSS {
	class GameObject : public CoreObject {
	public:
		//Events
		virtual bool handleEvent(GameEvent * event);
		
		/**
		 * Event Handlers
		 */
		
		//Transport
		virtual void eventTransportIncreased(TransportItem * transport) {}
		virtual void eventTransportDecreased(TransportItem * transport) {}
		
		//Hotel
		virtual void eventHotelVacated(HotelItem * hotel) {}
	};
}


#endif
