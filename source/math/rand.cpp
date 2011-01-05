#include "rand.h"

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
		return ((double)rand() / RAND_MAX) * (max - min) + min;
	}
}
