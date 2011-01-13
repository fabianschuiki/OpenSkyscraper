#include "basicresponder.h"

using namespace OSS;



void BasicResponder::registerEventHandler(string category, AbstractEventHandler * handler)
{
	eventHandlers[category] = handler;
}

bool BasicResponder::handleEvent(Event * event)
{
	if (!event) return false;
	
	//Check whether we have a registered handler for this event category.
	AbstractEventHandler * handler = eventHandlers[event->getCategory()];
	if (!handler)
		return false;
	
	//Call the event handler and return its result.
	return handler->handleEvent(event);
}

bool BasicResponder::sendEvent(Event * event)
{
	//Try to pass the event on to the next responders
	if (sendEventToNextResponders(event))
		return true;
	
	//Obviously none of our next responders was able to cope with the event, so let's try ourself
	return handleEvent(event);
}
