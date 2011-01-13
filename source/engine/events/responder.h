#ifndef OSS_ENGINE_RESPONDER_H
#define OSS_ENGINE_RESPONDER_H

#include "../external.h"
#include "basicresponder.h"


namespace OSS {	
	class SDLEvent;
	
	class MouseButtonEvent;
	class MouseDragEvent;
	class MouseMoveEvent;
	class ScrollWheelEvent;
	
	class KeyEvent;
	
	class AudioEvent;
	class VideoEvent;
	
	
	/**
	 * Engine Event Responder
	 *
	 * Provides dedicated handler functions for the engine events. Note that unlike any other
	 * dedicated responder class the engine's responder inherits from BasicResponder instead of
	 * ExtendingResponder. This is required since it is the lowest level responder around and
	 * there needs to be exactly one instance of BasicResponder in each responding classe's
	 * inheritance tree, for things like the eventHandlers map, etc..
	 *
	 * Important: If you design your own responder, make it inherit from ExtendingResponder!
	 */
	class Responder : public ExtendingResponder {
	public:
		Responder(BasicResponder * base);
		bool handleEngineEvent(Event * event);
		
		//SDL
		virtual bool eventSDL(SDLEvent * event) { return false; }
		
		//Mouse
		virtual bool eventMouseButton(MouseButtonEvent * event);
		virtual bool eventMouseDown(MouseButtonEvent * event) { return false; }
		virtual bool eventMouseUp(MouseButtonEvent * event) { return false; }
		virtual bool eventMouseDrag(MouseDragEvent * event) { return false; }
		virtual bool eventMouseMove(MouseMoveEvent * event) { return false; }
		virtual bool eventScrollWheel(ScrollWheelEvent * event) { return false; }
		
		//Key
		virtual bool eventKey(KeyEvent * event);
		virtual bool eventKeyDown(KeyEvent * event) { return false; }
		virtual bool eventKeyUp(KeyEvent * event) { return false; }
		
		//Audio
		virtual void eventAudioChanged(AudioEvent * event) {}
		virtual void eventVideoChanged(VideoEvent * event) {}
		
		//Scene
		virtual void eventMovedOnScreen(Event * event) {}
		virtual void eventMovedOffScreen(Event * event) {}
	};
}


#include "events/events.h"


#endif
