#ifndef OSS_BASE_RESPONDER_H
#define OSS_BASE_RESPONDER_H

#include "object.h"


namespace OSS {
	namespace Base {
		class Responder : public Object {
			/**
			 * Types
			 */
		public:
			//List
			typedef std::list< Pointer<Responder> > List;
			
			
			/**
			 * Event Sending
			 */
		public:
			bool sendEvent(Event * event);
			virtual const List & getNextResponders() { return List(); }
			
			
			/**
			 * Event Handling
			 */
		public:
			virtual bool handleEvent(Event * event) { return false; }
		};
	}
}


#endif
