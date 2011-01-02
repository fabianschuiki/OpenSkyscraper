#ifndef OSS_RECTMASK_H
#define OSS_RECTMASK_H

#include "../general.h"
#include "rect.h"


namespace OSS {
	namespace Math {
		template <typename T>
		
		class RectMask {
			typedef std::vector< Rect<T> * > RectList;
			typedef typename RectList::iterator ListIterator;
			RectList rects;
			
		public:
			std::string description() {
				std::string res = "(\n";
				for (ListIterator r = rects.begin(); r != rects.end(); r++) {
					res += "\t";
					res += (*r)->description();
					res += "\n";
				}
				res += ")";
				return res;
			}
			
			//Constructors
			RectMask() {}
			RectMask(Rect<T> * firstRect, ...) {
				va_list vl;
				va_start(vl, firstRect);
				Rect<T> * r = firstRect;
				while (r) {
					rects.push_back(r);
					r = va_arg(vl, Rect<T> *);
				}
				va_end(vl);
			}
			~RectMask() {
				for (ListIterator r = rects.begin(); r != rects.end(); r++)
					delete *r;
			}
			
			//Boolean operations
			bool containsPoint(const Vector2D<T> &p) const {
				if (rects.empty()) return true;
				for (ListIterator r = rects.begin(); r != rects.end(); r++)
					if ((*r)->containsPoint(p))
						return true;
				return false;
			}
			bool intersectsRect(const Rect<T> &r) const {
				if (rects.empty()) return true;
				for (ListIterator it = rects.begin(); it != rects.end(); it++)
					if ((*it)->intersectsRect(r))
						return true;
				return false;
			}
			
			//Area
			Rect<T> unionRect() const {
				if (rects.empty())
					return Rect<T>();
				ListIterator it = rects.begin();
				Rect<T> r = *(*it);
				for (; it != rects.end(); it++)
					r.unify(**it);
				return r;
			}
		};
	}
	
	//Convenience typedefs in the OSS namespace
	typedef Math::RectMask<int> rectmaski;
	typedef Math::RectMask<float> rectmaskf;
	typedef Math::RectMask<double> rectmaskd;
}


#endif
