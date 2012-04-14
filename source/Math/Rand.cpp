#include <cstdlib>
#include "Rand.h"

namespace OT {
	namespace Math {
		
		/// Returns a random integer between min and max (inclusive).
		int randi(int min, int max) 
		{
			return (rand() % (max - min + 1)) + min;
		}
		
		/// Returns a random double between min and max (inclusive).
		double randd(double min, double max)
		{
			return ((double)rand() / RAND_MAX) * (max - min) + min;
		}
	}
}
