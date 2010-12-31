#ifndef OSS_RAND_H
#define OSS_RAND_H

#include "../general.h"


namespace OSS {
	int randi(int min, int max);
	unsigned int randui(unsigned int min, unsigned int max);
	float randf(float min, float max);
	double randd(double min, double max);
}


#endif
