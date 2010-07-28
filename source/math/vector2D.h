#ifndef OSS_VECTOR2D_H
#define OSS_VECTOR2D_H

#include "../general.h"


namespace OSS {
	namespace Math {
		template <typename T> class Vector2D
		{
		public:
			T x;
			T y;
			
			//Description
			std::string description() {
				char str[32];
				sprintf(str, "(%f, %f)", (double)x, (double)y);
				return str;
			}
			
			//Conversion to frequently used types
			inline operator Vector2D<int>() const { return Vector2D<int>(x, y); }
			inline operator Vector2D<float>() const { return Vector2D<float>(x, y); }
			inline operator Vector2D<double>() const { return Vector2D<double>(x, y); }
			
			//Constructors
			Vector2D()
			{
				x = 0;
				y = 0;
			}
			Vector2D(T x, T y)
			{
				this->x = x;
				this->y = y;
			}
			
			//Operators
			Vector2D<T>& operator= (Vector2D<T> v)
			{
				x = v.x;
				y = v.y;
				return *this;
			}
			Vector2D<T> operator- ()	{ return Vector2D<T>(-x, -y); }
			
			//Vector-Vector
			Vector2D<T> operator+ (Vector2D<T> v)	{ return Vector2D<T>(x+v.x, y+v.y); }
			Vector2D<T> operator- (Vector2D<T> v)	{ return Vector2D<T>(x-v.x, y-v.y); }
			Vector2D<T> operator* (Vector2D<T> v)	{ return Vector2D<T>(x*v.x, y*v.y); }
			Vector2D<T> operator/ (Vector2D<T> v)	{ return Vector2D<T>(x/v.x, y/v.y); }
			//Vector-Scalar
			Vector2D<T> operator+ (T v)	{ return Vector2D<T>(x+v, y+v); }
			Vector2D<T> operator- (T v)	{ return Vector2D<T>(x-v, y-v); }
			Vector2D<T> operator* (T v)	{ return Vector2D<T>(x*v, y*v); }
			Vector2D<T> operator/ (T v)	{ return Vector2D<T>(x/v, y/v); }
			
			//Vector-Vector
			Vector2D<T>& operator+= (Vector2D<T> v)	{ x += v.x; y += v.y; return *this; }
			Vector2D<T>& operator-= (Vector2D<T> v)	{ x -= v.x; y -= v.y; return *this;	}
			Vector2D<T>& operator*= (Vector2D<T> v)	{ x *= v.x; y *= v.y; return *this; }
			Vector2D<T>& operator/= (Vector2D<T> v)	{ x /= v.x; y /= v.y; return *this; }
			
			//Vector-Scalar
			Vector2D<T>& operator+= (T v)	{ x += v; y += v; return *this; }
			Vector2D<T>& operator-= (T v)	{ x -= v; y -= v; return *this; }
			Vector2D<T>& operator*= (T v)	{ x *= v; y *= v; return *this; }
			Vector2D<T>& operator/= (T v)	{ x /= v; y /= v; return *this; }
			
			//Operations
			inline T length()
			{
				return sqrt(x*x + y*y);
			}
			
			double normalize()
			{
				T length = this->length();
				
				x /= length;
				y /= length;
				
				return length;
			}
			
			inline T dot(Vector2D<T> v)
			{
				return (x*v.x + y*v.y);
			}
		};
	}
	
	//Convenience typedefs in the OSS namespace
	typedef Math::Vector2D<int> int2;
	typedef Math::Vector2D<float> float2;
	typedef Math::Vector2D<double> double2;
}


#endif
