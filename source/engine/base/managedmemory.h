#ifndef OSS_ENGINE_BASE_MANAGEDMEMORY_H
#define OSS_ENGINE_BASE_MANAGEDMEMORY_H

#include "../external.h"


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
		void autorelease();
	};
}


#endif
