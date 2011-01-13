#include "responder.h"

using namespace OSS;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Engine Event Responder
//----------------------------------------------------------------------------------------------------

Responder::Responder()
{
	//Register our handler for engine events, so that we're able to call dedicated handler functions
	//for ease of use.
	registerEventHandler("engine", new EventHandler<Responder>(this, &Responder::handleEngineEvent));
}

bool Responder::handleEngineEvent(Event * event)
{
	//Check for various event types for which we have a dedicated handler function and call it, re-
	//turning its value.
	
	//SDL
	if (event->isType(Event::kSDL)) return eventSDL((SDLEvent *)event);
	
	//Mouse
	if (event->isType(Event::kMouseButton)) return eventMouseButton((MouseButtonEvent *)event);
	if (event->isType(Event::kMouseMove)) return eventMouseMove((MouseMoveEvent *)event);
	if (event->isType(Event::kMouseDrag)) return eventMouseDrag((MouseDragEvent *)event);
	if (event->isType(Event::kScrollWheel)) return eventScrollWheel((ScrollWheelEvent *)event);
	
	//Key
	if (event->isType(Event::kKey)) return eventKey((KeyEvent *)event);
	
	//Audio and Video updates
	if (event->isType(Event::kAudioChanged)) eventAudioChanged((AudioEvent *)event);
	if (event->isType(Event::kVideoChanged)) eventVideoChanged((VideoEvent *)event);
	
	//TODO: add dedicated handlers for scene events
	
	return false;
}

bool Responder::eventMouseButton(MouseButtonEvent * event)
{
	if (event->pressed)
		return eventMouseDown(event);
	else
		return eventMouseUp(event);
}

bool Responder::eventKey(KeyEvent * event)
{
	if (event->pressed)
		return eventKeyDown(event);
	else
		return eventKeyUp(event);
}
