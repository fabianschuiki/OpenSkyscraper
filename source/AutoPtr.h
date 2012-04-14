#pragma once
#include <cassert>
#include "Application.h"

namespace OT {
	template <typename T>
	class AutoPtr
	{
	protected:
		T * ptr;
	public:
		AutoPtr(T * ptr = NULL) { this->ptr = ptr; }
		AutoPtr(AutoPtr<T> & p) { ptr = p.ptr; p.ptr = NULL; }
		~AutoPtr()
		{
			if (ptr) delete ptr;
			ptr = NULL;
		}
		
		inline AutoPtr<T> & operator = (AutoPtr<T> & p)
		{
			if (ptr) delete ptr;
			ptr = p.ptr;
			p.ptr = NULL;
			return *this;
		}
		
		inline T * get() const { return ptr; }
		inline T & operator * () const { assert(ptr); return *ptr; }
		inline T * operator -> () const { assert(ptr); return *ptr; }
		
		inline bool operator < (const AutoPtr<T> & p) const { return ptr < p.ptr; }
	};
}