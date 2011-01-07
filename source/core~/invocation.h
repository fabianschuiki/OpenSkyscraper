#ifndef OSS_CORE_INVOCATION_H
#define OSS_CORE_INVOCATION_H

#include "../base/object~.h"


namespace OSS {
	namespace Core {
		/**
		 * Abstract Invocation
		 */
		class AbstractInvocation : public Base::Object {
		public:
			virtual void perform() = 0;
		};
		
		
		/**
		 * Invocation
		 */
		template <class T> class Invocation : public AbstractInvocation {
		protected:
			T* object;
			typedef void (T::*functionType)();
			functionType function;
			
		public:
			Invocation(T* object, functionType function)
			: object(object), function(function) {}
			
			void perform() { (object->*function)(); }
		};
	}
}


#endif
