#ifndef OSS_SINGLETON_H
#define OSS_SINGLETON_H

#include "../general.h"
#include "object.h"


namespace OSS {
	template <typename T, typename B> class Singleton : public B {
	private:
		static T* singleton;
		
	public:
		Singleton() {
			assert(!singleton);
			std::cerr << "** WARNING **  Using singleton of " << typeid(T).name()
			<< " which is BAD coding. You can do better than that!" << std::endl;
			
			//The next part is taken from "Enginuity, Part II", posted on gamedev.net
			//by Richard Fine. It is commented as follows:
			//	"use a cunning trick to get the singleton pointing to the start of the
			//		whole, rather than the start of the Singleton part of the object"
			long offset = (long)(T*)1 - (long)(Singleton<T, B>*)(T*)1;
			singleton = (T*)((long)this + offset);
		}
		
		~Singleton() {
			assert(singleton);
			singleton = 0;
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
	
	template <typename T, typename B> T* Singleton<T, B>::singleton = 0;
}


#endif
