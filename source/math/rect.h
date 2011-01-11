#ifndef OSS_RECT_H
#define OSS_RECT_H

#include "external.h"

#include "vector2D.h"


namespace OSS {
	namespace Math {
		template <typename T>
		
		class Rect : public Object {
		public:
			Vector2D<T> origin;
			Vector2D<T> size;
			
			std::string description() const {
				std::string res = "(";
				res += origin.description();
				res += ", ";
				res += size.description();
				res += ")";
				return res;
			}
			
			//Constructors
			Rect() : Object() {}
			Rect(T x, T y, T w, T h) : origin(x, y), size(w, h), Object() {}
			Rect(const Rect<T> &r) : origin(r.origin), size(r.size), Object() {}
			Rect(const Vector2D<T> origin, const Vector2D<T> size) :  Object() {
				this->origin = origin;
				this->size = size;
			}
			
			//Conversion to frequently used types
			inline operator Rect<int>() const { return Rect<int>(origin, size); }
			inline operator Rect<float>() const { return Rect<float>(origin, size); }
			inline operator Rect<double>() const { return Rect<double>(origin, size); }
			
			//Operators
			inline Rect<T>& operator =(const Rect<T> &r) {
				origin = r.origin;
				size = r.size;
				return *this;
			}
			inline Rect<T> operator -() const { return Rect<T>(-origin, -size); }
			
			//Boolean operators
			inline bool operator ==(const Rect<T> &r) const {
				return (origin == r.origin && size == r.size);
			}
			inline bool operator !=(const Rect<T> &r) const {
				return (origin != r.origin || size != r.size);
			}
			
			//Convenience coordinate accessors
			inline T minX() const { return origin.x; }
			inline T minY() const { return origin.y; }
			inline T midX() const { return origin.x + size.x / 2; }
			inline T midY() const { return origin.y + size.y / 2; }
			inline T maxX() const { return origin.x + size.x; }
			inline T maxY() const { return origin.y + size.y; }
			
			//Convenience point accessors
			inline Vector2D<T> minXminY() const { return Vector2D<T>(minX(), minY()); }
			inline Vector2D<T> minXmidY() const { return Vector2D<T>(minX(), midY()); }
			inline Vector2D<T> minXmaxY() const { return Vector2D<T>(minX(), maxY()); }
			inline Vector2D<T> midXminY() const { return Vector2D<T>(midX(), minY()); }
			inline Vector2D<T> center()	  const { return Vector2D<T>(midX(), midY()); }
			inline Vector2D<T> midXmaxY() const { return Vector2D<T>(midX(), maxY()); }
			inline Vector2D<T> maxXminY() const { return Vector2D<T>(maxX(), minY()); }
			inline Vector2D<T> maxXmidY() const { return Vector2D<T>(maxX(), midY()); }
			inline Vector2D<T> maxXmaxY() const { return Vector2D<T>(maxX(), maxY()); }
			
			//Area
			inline T area() const { return (size.x * size.y); }
			
			//Boolean operations
			bool containsPoint(const Vector2D<T> &p) const {
				return (p.x >= minX() && p.y >= minY() && p.x < maxX() && p.y < maxY());
			}
			bool intersectsRect(const Rect<T> &r) const {
				if (maxX() <= r.minX() || minX() >= r.maxX()) return false;
				if (maxY() <= r.minY() || minY() >= r.maxY()) return false;
				return true;
			}
			
			//Rect modification
			Rect<T> unionRect(const Rect<T> &r) const  {
				Rect<T> res(std::min<T>(minX(), r.minX()),
							std::min<T>(minY(), r.minY()),
							std::max<T>(maxX(), r.maxX()),
							std::max<T>(maxY(), r.maxY()));
				res.size.x -= res.origin.x;
				res.size.y -= res.origin.y;
				return res;
			}
			void unify(const Rect<T> &r) { *this = unionRect(r); }
			
			Rect<T> intersectionRect(const Rect<T> &r) const  {
				Rect<T> res(std::max<T>(minX(), r.minX()),
							std::max<T>(minY(), r.minY()),
							std::min<T>(maxX(), r.maxX()),
							std::min<T>(maxY(), r.maxY()));
				res.size.x -= res.origin.x;
				res.size.y -= res.origin.y;
				return res;
			}
			void intersect(const Rect<T> &r) { *this = intersectionRect(r); }
			
			Rect<T> insetRect(const Vector2D<T> & i) const {
				Rect<T> res(*this);
				res.origin += i;
				res.size -= i * 2;
				return res;
			}
			void inset(const Vector2D<T> & i) { *this = insetRect(i); }
			
			Rect<T> offsetRect(const Vector2D<T> & o) const {
				Rect<T> res(*this);
				res.origin += o;
				return res;
			}
			void offset(const Vector2D<T> & o) { *this = offsetRect(o); }
		};
		
	}
	
	//Convenience typedefs in the OSS namespace
	typedef Math::Rect<int> recti;
	typedef Math::Rect<float> rectf;
	typedef Math::Rect<double> rectd;
}


#endif
