#ifndef OSS_BASE_AUTORELEASEQUEUE_H
#define OSS_BASE_AUTORELEASEQUEUE_H


namespace OSS {
	namespace Base {
		class AutoreleaseQueue {
			
			static std::stack<AutoreleaseQueue *> autoreleaseQueueStack;
			AutoreleaseQueue();
			~AutoreleaseQueue();
			
			std::queue<Object *> objectQueue;
			static void autorelease(Object * object);
			void addObject(Object * object);
			void drain();
		}
	}
}


#endif
