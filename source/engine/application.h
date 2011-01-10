#ifndef OSS_ENGINE_APPLICATION_H
#define OSS_ENGINE_APPLICATION_H

#include "external.h"

#include "audio/audio.h"
#include "engine.h"
#include "eventpump.h"
#include "video/video.h"

//Include the external libraries
#ifdef __APPLE__
#include <SDL/SDL.h>
#include <IL/il.h>
#endif


namespace OSS {
	namespace Engine {
		class Application : public Core::Application, public Core::Currentable<Application> {			
			
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
