#include "responder.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Event Handling
//----------------------------------------------------------------------------------------------------

bool Classic::Responder::handleEvent(Base::Event * event)
{
	if (event->isKindOfClass(Class(Event)))
		return handleClassicEvent((Event *)event);
	return Engine::Responder::handleEvent(event);
}

bool Classic::Responder::handleClassicEvent(Classic::Event * event)
{
	if (event->isType(Event::TimeChanged))
		eventTimeChanged(event);
	if (event->isType(Event::DayChanged))
		eventDayChanged(event);
	if (event->isType(Event::QuarterChanged))
		eventQuarterChanged(event);
	if (event->isType(Event::YearChanged))
		eventYearChanged(event);
	
	if (event->isType(Event::TransportIncreased))
		eventTransportIncreased((ItemEvent<TransportItem> *)event);
	if (event->isType(Event::TransportDecreased))
		eventTransportDecreased((ItemEvent<TransportItem> *)event);
	
	if (event->isType(Event::HotelVacated))
		eventHotelVacated((ItemEvent<HotelItem> *)event);
	
	return false;
}
