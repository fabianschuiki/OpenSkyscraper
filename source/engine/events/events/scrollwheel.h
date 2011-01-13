#ifndef OSS_CORE_EVENTS_SCROLLWHEEL_H
#define OSS_CORE_EVENTS_SCROLLWHEEL_H

#include "mouse.h"


namespace OSS {
	class ScrollWheelEvent : public MouseEvent {
	public:
		const double2 displacement;	//scrolling amount
		
		ScrollWheelEvent(int2 position, double2 displacement)
		: MouseEvent(kScrollWheel, position), displacement(displacement) {}
	};
}


#endif
