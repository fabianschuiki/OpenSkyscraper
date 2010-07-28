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
	return resourcesPath() + "/" + resourceName;
}
