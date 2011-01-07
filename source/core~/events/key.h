#ifndef OSS_CORE_EVENTS_KEYEVENT_H
#define OSS_CORE_EVENTS_KEYEVENT_H

#include "../event.h"


namespace OSS {
	namespace Core {
		class KeyEvent : public Event {
		public:
			const bool pressed;				//whether the key was pressed or released
			const unsigned short unicode;	//unicode of the key pressed
			const bool isRepeat;			//whether this is a repeat event
			
			KeyEvent(bool pressed, unsigned short unicode, bool isRepeat)
			: Event(Event::Key), pressed(pressed), unicode(unicode), isRepeat(isRepeat) {}
		};
	}
}


#endif
