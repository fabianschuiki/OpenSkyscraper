#ifndef OSS_RECT_H
#define OSS_RECT_H

#include "../general.h"


namespace OSS {
	namespace Math {
		template <typename T>
		
		class Rect {
		public:
			Vector2D<T> origin;
			Vector2D<T> size;
			
			std::string description() {
				std::string res = "(";
				res += origin.description();
				res += ", ";
				res += size.description();
				res += ")";
				return res;
			}
			
			//Constructors
			Rect() {}
			Rect(T x, T y, T w, T h) : origin(x, y), size(w, h) {}
			Rect(const Rect<T> &r) : origin(r.origin), size(r.size) {}
			Rect(const Vector2D<T> origin, const Vector2D<T> size) {
				this->origin = origin;
				this->size = size;
			}
			
			//Conversion to frequently used types
			inline operator Rect<int>() const { return Rect<int>(origin, size); }
			inline operator Rect<float>() const { return Rect<float>(origin, size); }
			inline operator Rect<double>() const { return Rect<double>(origin, size); }
			
			//Operators
			inline Rect<T>& operator =(const Rect<T> r) {
				origin = r.origin;
				size = r.size;
				return *this;
			}
			inline Rect<T> operator -() { return Rect<T>(-origin, -size); }
			
			//Boolean operators
			inline bool operator ==(Rect<T> &r) {
				return (origin == r.origin && size == r.size);
			}
			inline bool operator !=(Rect<T> &r) {
				return (origin != r.origin || size != r.size);
			}
			
			//Convenience coordinate accessors
			inline T minX() { return origin.x; }
			inline T minY() { return origin.y; }
			inline T maxX() { return origin.x + size.x; }
			inline T maxY() { return origin.y + size.y; }
			
			//Boolean operations
			bool containsPoint(Vector2D<T> &p) {
				return (p.x >= minX() && p.y >= minY() && p.x < maxX() && p.y < maxY());
			}
			bool intersectsRect(Rect<T> &r) {
				if (maxX() <= r.minX() || minX() >= r.maxX()) return false;
				if (maxY() <= r.minY() || minY() >= r.maxY()) return false;
				return true;
			}
			
			//Rect modification
			Rect<T> unionRect(Rect<T> &r) {
				Rect<T> res(std::min<T>(minX(), r.minX()),
							std::min<T>(minY(), r.minY()),
							std::max<T>(maxX(), r.maxX()),
							std::max<T>(maxY(), r.maxY()));
				res.size.x -= res.origin.x;
				res.size.y -= res.origin.y;
				return res;
			}
			void unify(Rect<T> &r) { *this = unionRect(r); }
			
			Rect<T> intersectionRect(Rect<T> &r) {
				Rect<T> res(std::max<T>(minX(), r.minX()),
							std::max<T>(minY(), r.minY()),
							std::min<T>(maxX(), r.maxX()),
							std::min<T>(maxY(), r.maxY()));
				res.size.x -= res.origin.x;
				res.size.y -= res.origin.y;
				return res;
			}
			void intersect(Rect<T> &r) { *this = intersectionRect(r); }
			
			void inset(T x, T y) {
				origin.x += x;
				origin.y += y;
				size.x -= 2*x;
				size.y -= 2*y;
			}
		};
		
	}
	
	//Convenience typedefs in the OSS namespace
	typedef Math::Rect<int> recti;
	typedef Math::Rect<float> rectf;
	typedef Math::Rect<double> rectd;
}


#endif
