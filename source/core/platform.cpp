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
	//Get a list of paths where resources may be located
	std::vector<std::string> directories = resourcesPaths();
	
	//Iterate through the list
	for (std::vector<std::string>::iterator directory = directories.begin();
		 directory != directories.end();
		 directory++) {
		//If the file exists return the path
		std::string path = *directory + PATH_SEGMENTER + resourceName;
		if (access(path.c_str(), F_OK) != -1)
			return path;
	}
	
	//We were unlucky, log an error and return some thrash
	//TODO: do this in a more sophisticated way!
	OSSObjectError << "resource '" << resourceName << "' not found" << std::endl;
	return "";
}

std::string Platform::pathToResource(std::string resourceGroup, std::string resourceName)
{
	return pathToResource(resourceGroup + PATH_SEGMENTER + resourceName);
}

std::string Platform::pathToResource(std::string resourceGroup, std::string resourceName, std::string resourceType)
{
	return pathToResource(resourceGroup, resourceName + "." + resourceType);
}
