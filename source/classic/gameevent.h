#ifndef OSS_GAMEEVENT_H
#define OSS_GAMEEVENT_H

#include "../general.h"
#include "../core/coreevent.h"


namespace OSS {
	/**
	 * Game Events
	 */
	enum {
		//Transport
		kGameEventTransportIncreased = kNumEventsCore,
		kGameEventTransportDecreased,
		
		//Hotel
		kGameEventHotelVacated,
		
		kNumEventsGame
	};
	
	/**
	 * Transport events
	 */
	class TransportItem;
	typedef struct {
		unsigned int type;
		TransportItem * transport;
	} GameEventTransport;
	
	/**
	 * Hotel events
	 */
	class HotelItem;
	typedef struct {
		unsigned int type;
		HotelItem * hotel;
	} GameEventHotel;
	
	/**
	 * Game event union
	 */
	typedef union {
		unsigned int type;
		CoreEvent core;
		GameEventTransport transport;
		GameEventHotel hotel;
	} GameEvent;
}


#endif
