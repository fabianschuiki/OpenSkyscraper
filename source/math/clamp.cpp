#include "clamp.h"
#include <algorithm>


int (*clampi)(int, int, int) = &Math::clamp<int>;
float (*clampf)(float, float, float) = &Math::clamp<float>;
double (*clampd)(double, double, double) = &Math::clamp<double>;

const int & (*maxi)(const int &, const int &) = &std::max<int>;
const float & (*maxf)(const float &, const float &) = &std::max<float>;
const double & (*maxd)(const double &, const double &) = &std::max<double>;

const int & (*mini)(const int &, const int &) = &std::min<int>;
const float & (*minf)(const float &, const float &) = &std::min<float>;
const double & (*mind)(const double &, const double &) = &std::min<double>;
