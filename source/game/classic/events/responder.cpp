#include "responder.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Classic Event Responder
//----------------------------------------------------------------------------------------------------

Classic::Responder::Responder(BasicResponder * base) : ExtendingResponder(base)
{
	//Register our handler for classic events.
	base->registerEventHandler("classic", new EventHandler<Responder>(this, &Responder::handleClassicEvent));
}

bool Classic::Responder::handleClassicEvent(OSS::Event * engineEvent)
{
	//Check for the various event types.
	Event * event = (Event *)engineEvent;
	
	//Time
	if (event->isType(Event::kTimeChanged)) eventTimeChanged(event);
	if (event->isType(Event::kDayChanged)) eventDayChanged(event);
	if (event->isType(Event::kQuarterChanged)) eventQuarterChanged(event);
	if (event->isType(Event::kYearChanged)) eventYearChanged(event);
	
	//UI
	if (event->isType(Event::kToolChanged)) eventToolChanged(event);
	
	//Transport
	if (event->isType(Event::kTransportIncreased))
		eventTransportIncreased((ItemEvent<TransportItem> *)event);
	if (event->isType(Event::kTransportDecreased))
		eventTransportDecreased((ItemEvent<TransportItem> *)event);
	
	//Hotel
	if (event->isType(Event::kHotelVacated)) eventHotelVacated((ItemEvent<HotelItem> *)event);
	
	return false;
}
