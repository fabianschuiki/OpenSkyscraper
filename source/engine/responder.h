#ifndef OSS_ENGINE_RESPONDER_H
#define OSS_ENGINE_RESPONDER_H

#include "external.h"

#include "drawable.h"
#include "event.h"
#include "simulatable.h"


namespace OSS {
	namespace Engine {
		class Responder : public Core::Responder, public Drawable, public Simulatable {
			
			/**
			 * Event Handling
			 */
		public:
			virtual bool handleEvent(Base::Event * event);
			bool handleEngineEvent(Event * event);
			
			//Audio
			virtual void eventAudioChanged(AudioEvent * event) {}
			
			//Video
			virtual void eventVideoChanged(VideoEvent * event) {}
			
			//Active Scene
			virtual void eventMovedOnScreen(Event * event) {}
			virtual void eventMovedOffScreen(Event * event) {}
		};
	}
}


#endif
