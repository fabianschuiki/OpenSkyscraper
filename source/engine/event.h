#ifndef OSS_ENGINE_EVENT_H
#define OSS_ENGINE_EVENT_H

#include "../core~/event.h"


namespace OSS {
	namespace Engine {
		class Event : public Core::Event {
		public:
			synthesizeClassInheritance(Core::Event);
			
			
			/**
			 * Types
			 */
		public:
			typedef enum {
				None = 0,
				
				AudioChanged,
				VideoChanged,
				
				MovedOnScreen,
				MovedOffScreen
			} Type;
			
			
			/**
			 * Engine Event
			 */
		private:
			const Type engineType;
			
		public:
			Event() : Core::Event(), engineType(None) {}
			Event(Type t) : Core::Event(), engineType(t) {}
			
			bool isType(Type t) {
				return (engineType == t);
			}
		};
	}
}

//Include the various events
#include "events/audio.h"
#include "events/video.h"


#endif
