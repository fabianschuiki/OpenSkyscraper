#ifndef OSS_POINTER_H
#define OSS_POINTER_H


namespace OSS {
	namespace Base {
		template <class T> class Pointer {
			
			/**
			 * Construction
			 *
			 * The constructors reset the obj pointer to NULL. If an object or another Pointer in-
			 * stance is supplied, the overloaded assignment operator is used to set the pointer's
			 * value.
			 */
		protected:
			T* obj;
			
		public:
			//Constructs a pointer to NULL.
			Pointer() {
				obj = NULL;
			}
			
			//Constructs a pointer with a given pointer value.
			Pointer(T* o) {
				obj = NULL;
				*this = o;
			}
			
			//Constructs a pointer from another Pointer instance.
			Pointer(const Pointer<T>& p) {
				obj = NULL;
				*this = p;
			}
			
			//Destructs the pointer, releasing the object if there is one.
			~Pointer() {
				if (obj) obj->release();
			}
			
			
			/**
			 * Assignment
			 *
			 * Overloaded assignment operators that allow to use the instance as if it really was a
			 * pointer to the object.
			 */
		public:
			//Assign a pointer
			inline void operator =(T* o) {
				if (o) o->retain();
				if (obj) obj->release();
				obj = o;
			}
			
			//Assign another Pointer instance
			inline void operator =(const Pointer<T>& p) {
				if (p.obj) p.obj->retain();
				if (obj) obj->release();
				obj = p.obj;
			}
			
			
			/**
			 * Dereferencing
			 *
			 * Overloaded dereferencing operators that allow to use the instance as if it really was
			 * a pointer to the object.
			 */
		public:
			inline T& operator *() const {
				assert(obj != NULL && "tried to * on a NULL pointer");
				return *obj;
			}
			
			inline T* operator ->() const {
				assert(obj != NULL && "tried to -> on a NULL pointer");
				return obj;
			}
			
			
			/**
			 * Auto Typecast
			 *
			 * Makes the instance transparent and act as if it really was a pointer to the object.
			 */
		public:
			inline operator T*() const { return obj; }
			
			
			/**
			 * Logic
			 */
		public:
			inline bool operator !() { return !(obj); }
			inline bool operator ==(const Pointer<T>& p) const { return (obj == p.obj); }
			inline bool operator ==(const T* o) const { return (obj == o); }
		};
	}
}

//DEBUG:
using namespace OSS::Base;


#endif
