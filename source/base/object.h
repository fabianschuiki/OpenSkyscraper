#ifndef OSS_BASE_OBJECT_H
#define OSS_BASE_OBJECT_H

#include "external.h"
#include "managedmemory.h"

#include <typeinfo>


//Returns the class of a given object
#define Class(cls) typeid(cls)

//Synthesizes the isKindOfClass override required to support class inheritance checking.
#define synthesizeClassInheritance(baseclass, superclass) \
virtual bool isKindOfClass(Class c) { if (c == typeid(baseclass)) return true; \
return superclass::isKindOfClass(c); }

//Logging
#define OSSObjectLog std::cout << this->description() << " " << __FUNCTION__ << ": "
#define OSSObjectError std::cout << this->description() << " " << __FUNCTION__ << ": *** "


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
			std::string className() const;
			std::string instanceName() const;
			
			std::string description() const;
		};
	}
}


#endif
