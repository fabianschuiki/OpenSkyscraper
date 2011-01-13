#ifndef OSS_CORE_EVENTS_MOUSEMOVED_H
#define OSS_CORE_EVENTS_MOUSEMOVED_H

#include "mouse.h"


namespace OSS {
	class MouseMoveEvent : public MouseEvent {
	public:
		const int2 motion;	//relative motion of the mouse
		
		MouseMoveEvent(int2 position, int2 motion)
		: MouseEvent(kMouseMove, position), motion(motion) {}
	};
}


#endif
