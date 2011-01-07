#ifndef OSS_CORE_EVENTS_MOUSEBUTTON_H
#define OSS_CORE_EVENTS_MOUSEBUTTON_H

#include "mouse.h"


namespace OSS {
	namespace Core {
		class MouseButtonEvent : public MouseEvent {
		public:
			const unsigned int button;	//which button
			const bool pressed;			//whether the button is pressed at the moment
			
			MouseButtonEvent(int2 position, unsigned int button, unsigned int pressed)
			: MouseEvent(Event::MouseButton, position), button(button), pressed(pressed) {}
		};
	}
}


#endif
