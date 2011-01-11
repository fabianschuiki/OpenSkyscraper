#include "rand.h"

#include <cstdlib>

namespace OSS {
	int randi(int min, int max) {
		return (rand() % (max - min + 1)) + min;
	}
	
	unsigned int randui(unsigned int min, unsigned int max) {
		return (rand() % (max - min + 1)) + min;
	}
	
	float randf(float min, float max) {
		return ((float)rand() / RAND_MAX) * (max - min) + min;
	}
	
	double randd(double min, double max) {
		double f = ((double)rand() / RAND_MAX);
		assert(f <= 1 && f >= 0);
		return f * (max - min) + min;
	}
}
