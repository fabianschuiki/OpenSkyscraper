#pragma once

namespace Math {
	template <typename T> T clamp(T value, T min, T max) {
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}
}

//Convenience functions in the OSS namespace
extern int (*clampi)(int, int, int);
extern float (*clampf)(float, float, float);
extern double (*clampd)(double, double, double);

extern const int & (*maxi)(const int &, const int &);
extern const float & (*maxf)(const float &, const float &);
extern const double & (*maxd)(const double &, const double &);

extern const int & (*mini)(const int &, const int &);
extern const float & (*minf)(const float &, const float &);
extern const double & (*mind)(const double &, const double &);
