#include "application.h"

using namespace OSS;
using namespace Core;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Run Loop
//----------------------------------------------------------------------------------------------------

void Application::run()
{
	terminateReply = TerminateCancel;
	willRun();
	
	while (terminateReply != TerminateNow) {
		//Send the events from the pump down the responder chain
		pumpEvents();
		
		//If we're supposed to terminate, do so
		if (terminateReply == TerminateLater)
			terminate();
	}
	
	didRun();
}

bool Application::isRunning()
{
	return (terminateReply != TerminateNow);
}



void Application::terminate()
{
	terminateReply = shouldTerminate();
}

bool Application::isTerminating()
{
	return (terminateReply != TerminateCancel);
}

Application::TerminateReply Application::shouldTerminate()
{
	return TerminateNow;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Events
//----------------------------------------------------------------------------------------------------

void Application::pumpEvents()
{
	willPumpEvents();
	
	Event * event;
	if (eventPump)
		while ((event = eventPump->getNextEvent()))
			sendEvent(event);
	
	didPumpEvents();
}
