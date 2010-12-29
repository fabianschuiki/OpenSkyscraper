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
		std::vector<std::string> resourcesPaths();
		std::string pathToResource(std::string resourceName);
		std::string pathToResource(std::string resourceGroup, std::string resourceName);
		std::string pathToResource(std::string resourceGroup, std::string resourceName, std::string resourceType);
	};
}


#endif
