#ifndef OSS_CORE_EVENTS_MOUSEMOVED_H
#define OSS_CORE_EVENTS_MOUSEMOVED_H

#include "mouse.h"


namespace OSS {
	namespace Core {
		class MouseMovedEvent : public MouseEvent {
		public:
			const int2 motion;	//relative motion of the mouse
			
			MouseMovedEvent(int2 position, int2 motion)
			: MouseEvent(Event::MouseMoved, position), motion(motion) {}
		};
	}
}


#endif
