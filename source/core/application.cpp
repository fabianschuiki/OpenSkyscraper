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
	
	//The run loop gets its own autorelease queue
	Base::AutoreleaseQueue * runLoopGarbage = new Base::AutoreleaseQueue;
	
	while (terminateReply != TerminateNow) {
		//Notify
		willIterateRunLoop();
		
		//Send the events from the pump down the responder chain
		pumpEvents();
		
		//If we're supposed to terminate, do so
		if (terminateReply == TerminateLater)
			terminate();
		
		//Perform the invocations
		performInvocations();
		
		//Notify
		didIterateRunLoop();
		
		//Get rid of the garbage
		runLoopGarbage->drain();
	}
	
	//Get rid of the garbage queue
	delete runLoopGarbage;
	
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
#pragma mark Invocation
//----------------------------------------------------------------------------------------------------

void Application::addInvocation(AbstractInvocation * invocation)
{
	if (!invocation) return;
	invocations.push_back(invocation);
}

void Application::addInvocation(AbstractInvocation * invocation, OrderingMode ordered,
								AbstractInvocation * relativeTo)
{
	if (!invocation) return;
	if (!relativeTo) {
		addInvocation(invocation);
		return;
	}
	
	InvocationVector::iterator i;
	for (i = invocations.begin(); i != invocations.end(); i++) {
		if ((AbstractInvocation *)*i == relativeTo) {
			if (ordered == Before)
				invocations.insert(i, invocation);
			else
				invocations.insert((i + 1), invocation);
			return;
		}
	}
	
	invocations.insert(i, invocation);
}

void Application::removeInvocation(AbstractInvocation * invocation)
{
	if (!invocation) return;
	for (InvocationVector::iterator i = invocations.begin(); i != invocations.end(); i++)
		if ((AbstractInvocation *)*i == invocation)
			invocations.erase(i);
}

void Application::performInvocations()
{
	willPerformInvocations();
	for (InvocationVector::iterator i = invocations.begin(); i != invocations.end(); i++)
		(*i)->perform();
	didPerformInvocations();
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





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Resources
//----------------------------------------------------------------------------------------------------

std::string Application::pathToResource(std::string resourceName)
{
	//Get a list of paths where resources may be located
	vector<std::string> directories = resourcesPaths();
	
	//Iterate through the list
	for (vector<std::string>::iterator directory = directories.begin();
		 directory != directories.end();
		 directory++) {
		//If the file exists return the path
		std::string path = *directory + PATH_SEGMENTER + resourceName;
		if (access(path.c_str(), F_OK) != -1)
			return path;
	}
	
	//We were unlucky, log an error and return some thrash
	//TODO: do this in a more sophisticated way!
	OSSObjectError << "resource '" << resourceName << "' not found" << std::endl;
	return "";
}

std::string Application::pathToResource(std::string resourceGroup, std::string resourceName)
{
	return pathToResource(resourceGroup + PATH_SEGMENTER + resourceName);
}

std::string Application::pathToResource(std::string resourceGroup, std::string resourceName, std::string resourceType)
{
	return pathToResource(resourceGroup, resourceName + "." + resourceType);
}
