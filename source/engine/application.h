#ifndef OSS_ENGINE_APPLICATION_H
#define OSS_ENGINE_APPLICATION_H

#include "../core~/application.h"

#include "audio.h"
#include "engine.h"
#include "eventpump.h"
#include "video.h"

//This is where SDL is included
#ifdef __APPLE__
#include <SDL/SDL.h>
#endif


namespace OSS {
	namespace Engine {
		class Application : public Core::Application {			
			
			/**
			 * Construction
			 */
		public:
			Application();
			~Application();
			
			
			/**
			 * Subsystems
			 */
		public:
			Base::Pointer<EngineCore> engine;
			
			Base::Pointer<Video> video;
			Base::Pointer<Audio> audio;
			
			
			/**
			 * Event Sending
			 */
		public:
			virtual bool sendEventToNextResponders(Base::Event * event);
			
			
			/**
			 * Timing
			 */
		public:
			double getTimeElapsed();
			void sleep(double seconds);
		};
	}
}


#endif
