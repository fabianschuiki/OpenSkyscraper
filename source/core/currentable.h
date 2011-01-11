#ifndef OSS_CORE_CURRENTABLE_H
#define OSS_CORE_CURRENTABLE_H

#include "external.h"


namespace OSS {
	namespace Core {
		template <class T> class Currentable {
			
			/**
			 * Current Instance
			 *
			 * The Current Instance pattern provides a means of setting an instance of a given class
			 * as the default responder for subsequent messages. This is a possible solution to the
			 * problem of having to pass an instance of EngineCore, Audio, Video, or anything the
			 * like to all drawable entities in order to allow sound effects and similar things to
			 * be played. This is achieved by pushing the Engine and all its subsystems ontop of the
			 * instance stack just before calling the root simulate, update or draw function. Inside
			 * these functions individual objects may refer to the current instance of a class
			 * either by calling Class::getCurrent(), or using some shortcut static methods if the
			 * class provides any.
			 *
			 * Please note that this design pattern is probably considered ugly by some people since
			 * it involves a minimal amount of global state which is almost always a sign of bad
			 * software architecture.
			 */
		private:
			static std::stack<T *> instanceStack;
			
		public:
			void pushCurrent() {
				instanceStack.push((T *)this);
			}
			
			void popCurrent() {
				assert(instanceStack.top() == this && "instance not on top of stack");
				instanceStack.pop();
			}
			
			static T * getCurrent() {
				return instanceStack.top();
			}
		};
		
		//Required to initialize the instanceStack static variable.
		template <class T> std::stack<T *> Currentable<T>::instanceStack;
	}
}


#endif
