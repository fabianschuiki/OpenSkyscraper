#ifndef PLATFORM_H
#define PLATFORM_H

#include "../general.h"
#include "../base/base.h"


namespace OSS {
	class Platform : public Singleton<Platform, Object> {
	public:
		//Timing
		double getTimeElapsed();
		void sleep(double seconds);
	};
}


#endif
