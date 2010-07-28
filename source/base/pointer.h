/*
 *  pointer.h
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 24.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#ifndef OSS_POINTER_H
#define OSS_POINTER_H

namespace OSS {
	template <class T>
	class Pointer {
	protected:
		T* obj;
		
	public:
		//Constructors
		Pointer() {
			obj = NULL;
		}
		Pointer(T* o) {
			obj = NULL;
			*this = o;
		}
		Pointer(const Pointer<T> &p) {
			obj = NULL;
			*this = p;
		}
		
		//Destructor
		~Pointer() {
			if (obj) obj->release();
		}
		
		//Assignment operators
		inline void operator =(T* o) {
			if (obj) obj->release();
			obj = o;
			if (obj) obj->retain();
		}
		inline void operator =(const Pointer<T> &p) {
			if (obj) obj->release();
			obj = p.obj;
			if (obj) obj->retain();
		}
		
		//Accessing operators
		inline T& operator *() const {
			//assert(obj != NULL && "tried to * on a NULL pointer");
			return *obj;
		}
		inline T* operator ->() const {
			//assert(obj != NULL && "tried to -> on a NULL pointer");
			return obj;
		}
		
		//Automatically convert the pointer to T* if desired
		inline operator T*() const {
			return obj;
		}
		
		//Logic operators
		inline bool operator !() {
			return !(obj);
		}
		inline bool operator ==(const Pointer<T> &p) const {
			return (obj == p.obj);
		}
		inline bool operator ==(const T* o) const {
			return (obj == o);
		}
	};
}

#endif
