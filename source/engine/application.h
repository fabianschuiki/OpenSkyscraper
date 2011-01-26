#ifndef OSS_CORE_APPLICATION_H
#define OSS_CORE_APPLICATION_H

//Inheritance-induced includes
#include "external.h"
#include "base/object.h"
#include "base/singleton.h"
#include "events/basicresponder.h"

//Usage-induced includes
#include "base/pointer.h"
#include "base/autoreleasequeue.h"
#include "events/event.h"


namespace OSS {
	class Engine;
	
	class Application : public Object, public Singleton<Application>, public BasicResponder {		
		
		//The application maintains the engine singleton and provides its heartbeat.
	public:
		Pointer<Engine> engine;
		
		
		/**
		 * Initialization
		 */
	public:
		Application();
		
		
		/**
		 * Run Loop
		 *
		 * Calling run() will enter the run loop. The run loop will run as long as the terminate
		 * reply is not kTerminateNow. If you want to exit the application, call terminate(). This
		 * will ask the application shouldTerminate() to give subclasses and things like the engine
		 * the possibility to postpone (kTerminateLater) or cancel (kTerminateCancel) termination.
		 * This is important if you have unsaved changes in your tower, or anything the like.
		 */
	public:
		typedef enum {
			kTerminateCancel = 0,
			kTerminateNow,
			kTerminateLater
		} TerminateReply;
		
	private:
		TerminateReply terminateReply;
		
	public:
		void run();			
		bool isRunning();
		
		virtual void willRun() {}
		virtual void willIterateRunLoop() {}
		virtual void didIterateRunLoop() {}
		virtual void didRun() {}
		
		void terminate();
		bool isTerminating();
		TerminateReply shouldTerminate();
		
		
		/**
		 * Events
		 */
	private:
		Event * getNextEvent();
		
	public:
		void pumpEvents();
		
		virtual void willPumpEvents() {}
		virtual void didPumpEvents() {}
		
		virtual bool sendEventToNextResponders(Event * event);
		
		
		/**
		 * Resources
		 */
		
		//TODO: Move these functions to an appropriate location, for example the resource manager.
		vector<string> resourcesPaths();
		string pathToResource(string resourceName);
		string pathToResource(string resourceGroup, string resourceName);
		string pathToResource(string resourceGroup, string resourceName, string resourceType);
	};
}


#include "engine.h"


#endif
