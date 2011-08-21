#include <cstdlib>
#include "rand.h"


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
	return f * (max - min) + min;
}
