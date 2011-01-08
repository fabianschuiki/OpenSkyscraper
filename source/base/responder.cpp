#include "responder.h"

using namespace OSS;
using namespace Base;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Event Sending
//----------------------------------------------------------------------------------------------------

bool Responder::sendEvent(Event * event)
{
	//Try to pass the event on to the next responders
	if (sendEventToNextResponders(event))
		return true;
	
	//Obviously none of our next responders was able to cope with the event, so let's try ourself
	return handleEvent(event);
}
