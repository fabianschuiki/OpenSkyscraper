#ifndef OSS_ENGINE_EVENTS_HANDLER_H
#define OSS_ENGINE_EVENTS_HANDLER_H

#include "../external.h"
#include "../base/object.h"


namespace OSS {
	class Event;
	
	class AbstractEventHandler : public Object {
	public:
		virtual bool handleEvent(Event * event) = 0;
	};
	
	template <class T> class EventHandler : public AbstractEventHandler {
	private:
		T* object;
		typedef bool (T::*funcType)(Event * event);
		funcType function;
		
	public:
		EventHandler(T* object, funcType function) : object(object), function(function) {}
		
		bool handleEvent(Event * event) {
			return (object->*function)(event);
		}
	};
}


#endif
