#ifndef OSS_ENGINE_EVENTS_BASICRESPONDER_H
#define OSS_ENGINE_EVENTS_BASICRESPONDER_H

#include "../external.h"

#include "../base/pointer.h"
#include "handler.h"


namespace OSS {
	class Event;
	
	/**
	 * Abstract Responder
	 *
	 * The abstract responder defines a basic interface for event responders consisting of a
	 * function to register an event handler for a specific event category, and the basic event
	 * handling function.
	 *
	 * The registerEventHandler() function is used to define a function that is to be called
	 * when an event of a specific category is to be handled. This approach does not rely on
	 * subclasses overriding several virtual functions to extend the responder functionality,
	 * but rather defines a basic interface where subclasses may register their extended hand-
	 * ling capabilities. The following example shows why this is important:
	 *
	 *   class Engine::View : public Object, public Engine::Responder
	 *   class Game::View : public Engine::View, public Game::Responder
	 * 
	 * As you can see there is a problem: The engine defines a view class that handles basic
	 * drawing and responds to engine events. Now the game wants to take advantage of that and
	 * bases a new view class on the one defined by the engine. BUT the new class should also
	 * respond to game events. If the responder chain was implemented by relying on virtual
	 * function overriding, the dependency wouldn't work since both Engine:: and Game::Responder
	 * would override the same function -> disambiguity. Having both responder classes register
	 * their capabilities in their constructors will circumvent this problem since the decision
	 * whose handle function to call is made by the BasicResponder class, not by each responder
	 * subclass individually.
	 */
	class AbstractResponder {
	protected:
		virtual void registerEventHandler(string category, AbstractEventHandler * handler) = 0;
		
	public:
		virtual bool handleEvent(Event * event) = 0;
	};
	
	
	/**
	 * Basic Responder
	 *
	 * Implements the AbstractResponder interface and adds the eventHandlers map member variable
	 * that will hold a map of event handlers with their respective event category. The event
	 * handling function handleEvent() is implemented to check the event's category and call the
	 * appropriate event handler if possible.
	 *
	 * The sendEvent*() functions are used to create a responder chain. If you want to distri-
	 * bute an event, you call sendEvent() on the topmost object in the hierarchy. This function
	 * will try to pass the event on to the next responders using sendEventToNextResponders().
	 * If none was able to cope with the event, the responder's own handleEvent() function is
	 * called.
	 */
	class BasicResponder : public AbstractResponder {
	private:
		map<string, Pointer<AbstractEventHandler> > eventHandlers;
		
	public:
		void registerEventHandler(string category, AbstractEventHandler * handler);
		
		bool handleEvent(Event * event);
		
		bool sendEvent(Event * event);
		virtual bool sendEventToNextResponders(Event * event) { return false; }
	};
}


#include "event.h"


#endif
