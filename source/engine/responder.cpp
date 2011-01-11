#include "responder.h"

using namespace OSS;
using namespace Engine;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Event Handling
//----------------------------------------------------------------------------------------------------

bool Engine::Responder::handleEvent(Base::Event * event)
{
	if (event->isKindOfClass(Class(Event)))
		return handleEngineEvent((Event *)event);
	return Core::Responder::handleEvent(event);
}

bool Engine::Responder::handleEngineEvent(Engine::Event * event)
{
	if (event->isType(Event::AudioChanged))
		eventAudioChanged((AudioEvent *)event);
	if (event->isType(Event::VideoChanged))
		eventVideoChanged((VideoEvent *)event);
	
	return false;
}
