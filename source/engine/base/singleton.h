#ifndef OSS_ENGINE_BASE_SINGLETON_H
#define OSS_ENGINE_BASE_SINGLETON_H

#include "../external.h"


namespace OSS {
	template <class T> class Singleton {
		
		static T* singleton;
		
	public:
		Singleton() {
			assert(!singleton);
			
			//The next part is taken from "Enginuity, Part II", posted on gamedev.net
			//by Richard Fine. It is commented as follows:
			//	"use a cunning trick to get the singleton pointing to the start of the
			//		whole, rather than the start of the Singleton part of the object"
			long offset = (long)(T*)1 - (long)(Singleton<T>*)(T*)1;
			singleton = (T*)((long)this + offset);
		}
		
		~Singleton() {
			assert(singleton);
			singleton = NULL;
		}
		
		static T* shared() {
			assert(singleton);
			return singleton;
		}
		static T& sharedRef() {
			assert(singleton);
			return *singleton;
		}
	};
	
	template <class T> T* Singleton<T>::singleton = NULL;
}


#endif
