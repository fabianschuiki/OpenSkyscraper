#ifndef OSS_ENGINE_H
#define OSS_ENGINE_H

#include "../core~/responder.h"
#include "../core~/invocation.h"

#include "cruisecontrol.h"


namespace OSS {
	namespace Engine {
		class Application;
		
		class EngineCore : public Core::Responder {
			
			/**
			 * Construction
			 */
		public:
			EngineCore(Application * application);
			
			
			/**
			 * Run Loop Attachment
			 */
		private:
			Base::Pointer< Core::Invocation<EngineCore> > invocation;
			
		public:
			Base::Pointer<Application> application;
			
			void attachToApplication(Application * app);
			void detachFromApplication();
			
			virtual void willAttachToApplication(Application * application) {}
			virtual void didAttachToApplication() {}
			
			
			/**
			 * State
			 */
		public:
			virtual void update();
			
			
			/**
			 * Timing
			 */
		public:
			Base::Pointer<CruiseControl> timing;
		};
	}
}


#endif
