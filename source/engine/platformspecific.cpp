#include "external.h"

#ifndef PLATFORM_APPLE

#include "application.h"
vector<string> OSS::Core::Application::resourcesPaths()
{
	vector<string> paths;
	paths.push_back(".");
	paths.push_back("./resources");
	return paths;
}

#endif

#ifdef PLATFORM_WINDOWS
	#define PATH_SEGMENTER "\\"
#else
	#define PATH_SEGMENTER "/"
#endif

#include <unistd.h>

string OSS::Core::Application::pathToResource(string resourceName)
{
	//Get a list of paths where resources may be located
	vector<string> directories = resourcesPaths();
	
	//Iterate through the list
	for (vector<string>::iterator directory = directories.begin();
		 directory != directories.end();
		 directory++) {
		//If the file exists return the path
		string path = *directory + PATH_SEGMENTER + resourceName;
		if (access(path.c_str(), F_OK) != -1)
			return path;
	}
	
	//We were unlucky, log an error and return some thrash
	//TODO: do this in a more sophisticated way!
	OSSObjectError << "resource '" << resourceName << "' not found" << std::endl;
	return "";
}

string OSS::Core::Application::pathToResource(string resourceGroup, string resourceName)
{
	return pathToResource(resourceGroup + PATH_SEGMENTER + resourceName);
}