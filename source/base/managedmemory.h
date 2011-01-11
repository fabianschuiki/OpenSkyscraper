#ifndef OSS_BASE_MANAGEDMEMORY_H
#define OSS_BASE_MANAGEDMEMORY_H

#include <cstddef> //for operator new, delete
#include "external.h"


namespace OSS {
	namespace Base {
		class ManagedMemory {
			
			/**
			 * Construction
			 */
		public:
			ManagedMemory();
			virtual ~ManagedMemory();
			
			
			/**
			 * Reference Counting
			 */
		private:
			int retainCount;
			
		public:
			ManagedMemory * retain();
			ManagedMemory * release();
			ManagedMemory * autoretain();
			ManagedMemory * autorelease();
			
			
			/**
			 * Memory Allocation
			 */			
		public:			
			void * operator new(std::size_t size);
			void operator delete(void * ptr);
		};
	}
}


#endif
