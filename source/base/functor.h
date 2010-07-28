/*
 *  functor.h
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 27.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#ifndef OSS_FUNCTOR_H
#define OSS_FUNCTOR_H
#include "object.h"

namespace OSS {
	template <typename A>
	class Functor : public Object {
	public:
		virtual void operator ()(A arg) = 0;
	};
	
	template <class T, typename A>
	class ObjectFunctor : public Functor<A> {
	protected:
		T* object;
		typedef void (T::*FuncType)(A arg);
		FuncType function;
		
	public:
		ObjectFunctor(T* o, FuncType f) {
			object = o;
			function = f;
		}
		
		void operator ()(A arg) {
			(object->*function)(arg);
		}
	};
}

#endif
