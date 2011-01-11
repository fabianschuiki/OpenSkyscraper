#ifndef OSS_BASE_AUTORELEASEQUEUE_H
#define OSS_BASE_AUTORELEASEQUEUE_H

#include "external.h"


namespace OSS {
	namespace Base {
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
}


#endif
