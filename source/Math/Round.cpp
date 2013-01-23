#include "Round.h"

#ifdef _MSC_VER
int round (float x)
{
	const float    round_to_nearest = 0.5f;
	int            i;
	__asm
	{
		fld   x
		fadd  st, st (0)
		fadd  round_to_nearest
		fistp i
		sar   i, 1
	}
	return (i);  
}
#elif _WIN32
#include <cmath>
/*inline float round(float x) { return x < 0.0f ? ceil(x - 0.5f) : floor(x + 0.5f); }
inline double round(double x) { return x < 0.0 ? ceil(x - 0.5) : floor(x + 0.5); }*/
inline int round(float x) { return x < 0.0f ? ceil(x - 0.5f) : floor(x + 0.5f); }
#endif
