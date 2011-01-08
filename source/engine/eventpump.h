#ifndef OSS_ENGINE_EVENTPUMP_H
#define OSS_ENGINE_EVENTPUMP_H

#include "../core/eventpump.h"

#include "../base/pointer.h"


namespace OSS {
	namespace Engine {
		class Application;
		
		class EventPump : public Core::EventPump {
			
			/**
			 * Construction
			 */
		public:
			const Base::Pointer<Application> application;
			
			EventPump(Application * application);
			
			
			/**
			 * Event Pumping
			 */
			Core::Event * getNextEvent();
		};
	}
}


#endif
