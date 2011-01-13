#ifndef OSS_CORE_EVENTS_MOUSEDRAGGED_H
#define OSS_CORE_EVENTS_MOUSEDRAGGED_H

#include "mouse.h"


namespace OSS {
	class MouseDragEvent : public MouseEvent {
	public:
		const int2 origin;		//where the dragging initially started
		const bool dragging;	//whether the dragging is still occuring or has ended
		
		MouseDragEvent(int2 position, int2 origin, bool dragging)
		: MouseEvent(kMouseDrag, position), origin(origin), dragging(dragging) {}
	};
}


#endif
