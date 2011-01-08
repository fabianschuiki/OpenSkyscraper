#ifndef OSS_BASE_OBJECT_H
#define OSS_BASE_OBJECT_H

#include "managedmemory.h"

#include <string>
#include <typeinfo>


//Returns the class of a given object
#define Class(cls) typeid(cls)

//Synthesizes the isKindOfClass override required to support class inheritance checking.
#define synthesizeClassInheritance(superclass) \
virtual bool isKindOfClass(Class c) { if (c == typeid(this)) return true; \
return superclass::isKindOfClass(c); }


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
			
			
			/**
			 * Description
			 *
			 * Methods related to describing objects in human-readable form.
			 */
		public:
			std::string className();
			std::string instanceName();
			
			std::string description();
		};
	}
}


#endif
