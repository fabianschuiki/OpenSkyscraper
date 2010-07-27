#include "coreobject.h"

using namespace OSS;


bool CoreObject::handleEvent(CoreEvent * event)
{
	if (Object::handleEvent(&event->base)) return true;
	switch (event->type) {
		case kCoreEventPrepare:	return eventPrepare(); break;
		case kCoreEventCleanUp:	return eventCleanUp(); break;
			
		case kCoreEventVideoModeChanged:	return eventVideoModeChanged(); break;
	}
	return false;
}
