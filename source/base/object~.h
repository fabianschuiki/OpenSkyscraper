#ifndef OSS_BASE_OBJECT_H
#define OSS_BASE_OBJECT_H

#include "managedmemory.h"

#include <typeinfo>

#define Class(cls) typeid(cls)


namespace OSS {
	namespace Base {
		class Object : public ManagedMemory {
			
			/**
			 * Class Inheritance
			 */
		public:
			typedef const std::type_info & Class;
			
			Class getClass();
			
			bool isMemberOfClass(Class c);
			virtual bool isKindOfClass(Class c);
		};
	}
}


#endif
