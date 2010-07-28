#ifndef OSS_PLATFORM_H
#define OSS_PLATFORM_H

#include "../general.h"
#include "../base/base.h"


namespace OSS {
	class Platform : public Singleton<Platform, Object> {
	public:
		//Timing
		double getTimeElapsed();
		void sleep(double seconds);
		
		//Paths
		std::string resourcesPath();
		std::string pathToResource(std::string resourceName);
	};
}


#endif
