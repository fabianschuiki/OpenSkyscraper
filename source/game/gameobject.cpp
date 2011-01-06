#include "gameobject.h"

using namespace OSS;


bool GameObject::handleEvent(GameEvent * event)
{
	if (CoreObject::handleEvent(&event->core)) return true;
	switch (event->type) {
		case kGameEventTransportIncreased:	eventTransportIncreased(event->transport.transport); break;
		case kGameEventTransportDecreased:	eventTransportDecreased(event->transport.transport); break;
			
		case kGameEventHotelVacated:		eventHotelVacated(event->hotel.hotel); break;
	}
	return false;
}
