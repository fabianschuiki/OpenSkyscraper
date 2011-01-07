#ifndef OSS_CORE_APPLICATION_H
#define OSS_CORE_APPLICATION_H

#include "responder.h"

#include "eventpump.h"
#include "../base/pointer.h"


namespace OSS {
	namespace Core {
		class Application : public Base::Responder {
			
			/**
			 * Types
			 */
		public:
			typedef enum {
				TerminateCancel = 0,
				TerminateNow,
				TerminateLater
			} TerminateReply;
			
			
			/**
			 * Run Loop
			 */
		private:
			TerminateReply terminateReply;
			
		public:
			void run();			
			bool isRunning();
			
			virtual void willRun() {}
			virtual void didRun() {}
			
			void terminate();
			bool isTerminating();
			TerminateReply shouldTerminate();
			
			
			/**
			 * Invocation
			 *
			 * The invocation system provides a means of queuing function calls in the application
			 * run loop. A game engine might for example add a periodic invocation to hook into the
			 * run loop to provide timing and animation.
			 */
		private:
			typedef std::vector< Base::Pointer<Invocation> > InvocationVector;
			InvocationVector invocations;
		public:
			void addInvocation(Invocation * invocation);
			
			
			/**
			 * Events
			 */
		private:
			Base::Pointer<EventPump> eventPump;
			
		public:
			void pumpEvents();
			
			virtual void willPumpEvents() {}
			virtual void didPumpEvents() {}
		};
	}
}


#endif
