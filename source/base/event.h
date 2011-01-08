#ifndef OSS_BASE_EVENT_H
#define OSS_BASE_EVENT_H

#include "object.h"


namespace OSS {
	namespace Base {
		class Event : public Object {
			
			/**
			 * Types
			 *
			 * Every class inheriting from Base::Event should have its own typedef'd enumeration of
			 * event types. They need to have at least the None type.
			 */
		public:
			typedef enum {
				None = 0
			} Type;
			
			
			/**
			 * Base Event
			 *
			 * The base event is just a placeholder and example for how to create other event
			 * classes inheriting from this. The idea is that every subclass defines its own
			 * const type member variable, with an appropriate name (coreType, gameType, etc.).
			 * Two constructors must exist: An empty one which initializes the instance to the None
			 * event type, and one taking the event type as an argument.
			 *
			 * The isType functions is the key element of the event structure. Every subclass should
			 * implement it for their own Type enum type. It should return whether the subclass's
			 * type member variable equals the parameter.
			 *
			 * Organizing events like this it is possible to have subclasses of events without them
			 * knowing anything about the superclass's event types. For example, asking Core::Event
			 * whether isType(someBaseEventType) will return false, since the Core::Event called the
			 * Base::Event's empty constructor.
			 */
		private:
			const Type baseType;
			
		public:
			Event() : baseType(None) {}
			Event(Type t) : baseType(t) {}
			
			bool isType(Type t) {
				return (baseType == t);
			}
		};
	}
}


#endif
