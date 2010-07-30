#include "coreobject.h"

using namespace OSS;


bool CoreObject::handleEvent(CoreEvent * event)
{
	if (Object::handleEvent(&event->base)) return true;
	switch (event->type) {
		case kCoreEventPrepare:	eventPrepare(); break;
		case kCoreEventCleanUp:	eventCleanUp(); break;
			
		case kCoreEventVideoModeChanged:	eventVideoModeChanged(); break;
			
		case kCoreEventSceneMovedOnScreen:	eventSceneMovedOnScreen(event->scene.scene); break;
		case kCoreEventSceneMovedOffScreen:	eventSceneMovedOffScreen(event->scene.scene); break;
	}
	return false;
}
