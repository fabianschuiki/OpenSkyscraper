#include "external.h"

#ifndef PLATFORM_APPLE

#include "application.h"
std::vector<std::string> OSS::Core::Application::resourcesPaths()
{
	std::vector<std::string> paths;
	paths.push_back(".");
	paths.push_back("./resources");
	return paths;
}

#endif