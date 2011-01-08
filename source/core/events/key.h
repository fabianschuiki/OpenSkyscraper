#ifndef OSS_CORE_EVENTS_KEYEVENT_H
#define OSS_CORE_EVENTS_KEYEVENT_H

#include "../event.h"


namespace OSS {
	namespace Core {
		class KeyEvent : public Event {
		public:
			const unsigned short unicode;	//unicode of the key pressed
			const bool pressed;				//whether the key was pressed or released
			const bool isRepeat;			//whether this is a repeat event
			
			KeyEvent(unsigned short unicode, bool pressed, bool isRepeat)
			: Event(Event::Key), unicode(unicode), pressed(pressed), isRepeat(isRepeat) {}
		};
	}
}


#endif
