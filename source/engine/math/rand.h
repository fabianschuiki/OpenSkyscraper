#ifndef OSS_ENGINE_MATH_RAND_H
#define OSS_ENGINE_MATH_RAND_H

#include "../external.h"


namespace OSS {
	int randi(int min, int max);
	unsigned int randui(unsigned int min, unsigned int max);
	float randf(float min, float max);
	double randd(double min, double max);
}


#endif
