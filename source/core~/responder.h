#ifndef OSS_CORE_RESPONDER_H
#define OSS_CORE_RESPONDER_H

#include "../base/responder.h"

#include "event.h"


namespace OSS {
	namespace Core {
		class Responder : public Base::Responder {
			
			/**
			 * Handling
			 */
		public:
			virtual bool handleEvent(Base::Event * event);
			bool handleCoreEvent(Event * event);
			
			//Mouse Button
			virtual bool eventMouseButton(MouseButtonEvent * event);
			virtual bool eventMouseDown(MouseButtonEvent * event) { return false; }
			virtual bool eventMouseUp(MouseButtonEvent * event) { return false; }
			
			//Mouse Dragged
			virtual bool eventMouseDragged(MouseDraggedEvent * event) { return false; }
			
			//Mouse Moved
			virtual bool eventMouseMoved(MouseMovedEvent * event) { return false; }
			
			//Scroll Wheel
			virtual bool eventScrollWheel(ScrollWheelEvent * event) { return false; }
			
			//Key
			virtual bool eventKey(KeyEvent * event);
			virtual bool eventKeyDown(KeyEvent * event) { return false; }
			virtual bool eventKeyUp(KeyEvent * event) { return false; }
		};
	}
}


#endif
