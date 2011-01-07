#ifndef OSS_POINTER_H
#define OSS_POINTER_H


namespace OSS {
	namespace Base {
		template <class T> class Pointer {
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
				if (o) o->retain();
				if (obj) obj->release();
				obj = o;
			}
			inline void operator =(const Pointer<T> &p) {
				if (o) o->retain();
				if (obj) obj->release();
				obj = p.obj;
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
}

//DEBUG:
using namespace OSS::Base;


#endif
