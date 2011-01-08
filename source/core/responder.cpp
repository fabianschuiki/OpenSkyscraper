#include "responder.h"

using namespace OSS;
using namespace Core;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Event Handling
//----------------------------------------------------------------------------------------------------

bool Responder::handleEvent(Base::Event * event)
{
	if (event->isKindOfClass(Class(Event)))
		return handleCoreEvent((Event *)event);
	return Base::Responder::handleEvent(event);
}

bool Responder::handleCoreEvent(Core::Event * event)
{
	if (event->isType(Event::MouseButton))
		if (eventMouseButton((MouseButtonEvent *)event)) return true;
	if (event->isType(Event::MouseMoved))
		if (eventMouseMoved((MouseMovedEvent *)event)) return true;
	if (event->isType(Event::MouseDragged))
		if (eventMouseDragged((MouseDraggedEvent *)event)) return true;
	if (event->isType(Event::ScrollWheel))
		if (eventScrollWheel((ScrollWheelEvent *)event)) return true;
	if (event->isType(Event::Key))
		if (eventKey((KeyEvent *)event)) return true;
	
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
