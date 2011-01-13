#ifndef OSS_ENGINE_BASE_AUTORELEASEQUEUE_H
#define OSS_ENGINE_BASE_AUTORELEASEQUEUE_H

#include "../external.h"


namespace OSS {
	class ManagedMemory;
	
	class AutoreleaseQueue {
		
		/**
		 * Nesting
		 */
	private:
		static std::stack<AutoreleaseQueue *> stack;
		
	public:
		AutoreleaseQueue();
		~AutoreleaseQueue();
		
		
		/**
		 * Autoreleasing
		 */
	private:
		std::queue<ManagedMemory *> garbageQueue;
		void addGarbage(ManagedMemory * garbage);
		
	public:
		static void autorelease(ManagedMemory * garbage);
		void drain();
	};
}


#endif
