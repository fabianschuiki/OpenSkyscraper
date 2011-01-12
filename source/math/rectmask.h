#ifndef OSS_RECTMASK_H
#define OSS_RECTMASK_H

#include "external.h"
#include "rect.h"

#include <cstdarg>

namespace OSS {	
	namespace Math {
		template <typename T>
		
		class RectMask {
			typedef vector< Rect<T> > RectList;
			typedef typename RectList::iterator ListIterator;
			typedef typename RectList::const_iterator ListConstIterator;
			RectList rects;
			
		public:
			string description() {
				string res = "(\n";
				for (ListIterator r = rects.begin(); r != rects.end(); r++) {
					res += "\t";
					res += (*r).description();
					res += "\n";
				}
				res += ")";
				return res;
			}
			
			//Constructors
			RectMask() {}
			RectMask(const Rect<T> * firstRect, ...) {
				va_list vl;
				va_start(vl, firstRect);
				const Rect<T> * r = firstRect;
				while (r) {
					rects.push_back(*r);
					r = va_arg(vl, Rect<T> *);
				}
				va_end(vl);
			}
			
			//Boolean operations
			bool containsPoint(const Vector2D<T> &p) const {
				if (rects.empty()) return true;
				for (ListConstIterator it = rects.begin(); it != rects.end(); it++)
					if ((*it).containsPoint(p))
						return true;
				return false;
			}
			bool intersectsRect(const Rect<T> &r) const {
				if (rects.empty()) return true;
				for (ListConstIterator it = rects.begin(); it != rects.end(); it++)
					if ((*it).intersectsRect(r))
						return true;
				return false;
			}
			bool empty() const {
				return rects.empty();
			}
			
			//Area
			Rect<T> bounds() const {
				if (rects.empty())
					return Rect<T>();
				ListConstIterator it = rects.begin();
				Rect<T> r = *(it++);
				for (; it != rects.end(); it++)
					r.unify(*it);
				return r;
			}
			
			inline T area() const {
				T a = 0;
				for (ListConstIterator it = rects.begin(); it != rects.end(); it++)
					a += (*it).area();
				return a;
			}
			
			//Rect modification
			RectMask<T> offsetRectMask(const Vector2D<T> & o) const  {
				RectMask<T> res(*this);
				for (ListIterator it = res.rects.begin(); it != res.rects.end(); it++)
					(*it).offset(o);
				return res;
			}
			void offset(const Vector2D<T> & o) { *this = offsetRectMask(o); }
		};
	}
	
	//Convenience typedefs in the OSS namespace
	typedef Math::RectMask<int> rectmaski;
	typedef Math::RectMask<float> rectmaskf;
	typedef Math::RectMask<double> rectmaskd;
}


#endif
