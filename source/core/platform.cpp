#include "platform.h"

using namespace OSS;


//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Timing
//----------------------------------------------------------------------------------------------------
double Platform::getTimeElapsed()
{
	return ((double)SDL_GetTicks()) / 1000;
}

void Platform::sleep(double seconds)
{
	SDL_Delay(seconds * 1000);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Paths
//----------------------------------------------------------------------------------------------------

std::string Platform::pathToResource(std::string resourceName)
{
	return resourcesPath() + PATH_SEGMENTER + resourceName;
}

std::string Platform::pathToResource(std::string resourceGroup, std::string resourceName)
{
	return pathToResource(resourceGroup) + PATH_SEGMENTER + resourceName;
}

std::string Platform::pathToResource(std::string resourceGroup, std::string resourceName, std::string resourceType)
{
	return pathToResource(resourceGroup, resourceName) + "." + resourceType;
}
