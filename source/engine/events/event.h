#ifndef OSS_ENGINE_EVENT_H
#define OSS_ENGINE_EVENT_H

#include "../external.h"
#include "../base/object.h"

#include "../base/pointer.h"


namespace OSS {
	class Event : public Object {
	public:
		typedef enum {
			kNoEvent = 0,
			
			//SDL events unable to be categorized
			kSDL,
			
			//Mouse
			kMouseButton,
			kMouseDrag,
			kMouseMove,
			kScrollWheel,
			
			//Keyboard
			kKey,
			
			//Audio and Video updates
			kAudioChanged,
			kVideoChanged,
			
			//Scene
			kMovedOnScreen,
			kMovedOffScreen
		} Type;
		
	private:
		const Type engineType;
		
	public:
		Event() : Object(), engineType(kNoEvent) {}
		Event(Type t) : Object(), engineType(t) {}
		
		bool isType(Type t) {
			return (engineType == t);
		}
		virtual string getCategory() { return "engine"; }
	};
}


#endif
