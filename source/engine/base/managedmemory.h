#ifndef OSS_ENGINE_BASE_MANAGEDMEMORY_H
#define OSS_ENGINE_BASE_MANAGEDMEMORY_H

#include "../external.h"

#include <cstdlib>


namespace OSS {
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
		void retain();
		void release();
		void autoretain();
		virtual void autorelease();
		
		//DEBUG: get rid of this again! This code makes sure that all classes initialize to all-zero
		//since a few of the older classes rely on that functionality. BAD CODING!
		void * operator new(std::size_t size)
		{
			void * memory = calloc(1, size);
			assert(memory);
			return memory;
		}
		
		void operator delete(void * ptr)
		{
			free(ptr);
		}
	};
}


#endif
